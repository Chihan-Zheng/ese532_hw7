#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

#include <CL/cl2.hpp>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "Utilities.h"

int main(int argc, char *argv[])
{
    EventTimer timer1, timer2;
    timer1.add("Main program");

    std::cout << "Running " << FRAMES << " iterations of " << "task" << std::endl;
    // ------------------------------------------------------------------------------------
    // Step 1: Initialize the OpenCL environment
     // ------------------------------------------------------------------------------------
    timer2.add("OpenCL Initialization");
    cl_int err;
    std::string binaryFile = argv[1];
    unsigned fileBufSize;
    std::vector<cl::Device> devices = get_xilinx_devices();
    devices.resize(1);
    cl::Device device = devices[0];
    cl::Context context(device, NULL, NULL, NULL, &err);
    char *fileBuf = read_binary_file(binaryFile, fileBufSize);
    cl::Program::Binaries bins{{fileBuf, fileBufSize}};
    cl::Program program(context, devices, bins, NULL, &err);
    cl::CommandQueue q(context, device, CL_QUEUE_PROFILING_ENABLE | CL_QUEUE_OUT_OF_ORDER_EXEC_MODE_ENABLE, &err);
    cl::Kernel krnl_filter(program, "Filter_HW", &err);

    // ------------------------------------------------------------------------------------
    // Step 2: Create buffers and initialize test values
    // ------------------------------------------------------------------------------------
    timer2.add("Allocate contiguous OpenCL buffers");

    size_t Input_buf_size = SCALED_FRAME_HEIGHT * SCALED_FRAME_WIDTH;
    size_t Output_buf_size = OUTPUT_FRAME_HEIGHT * OUTPUT_FRAME_WIDTH;

    cl::Buffer Input_buf[FRAMES];
    cl::Buffer Output_buf[FRAMES];
 
    for(int i = 0; i < FRAMES; i++)
    {
        Input_buf[i] = cl::Buffer(context, CL_MEM_READ_ONLY, Input_buf_size, NULL, &err);
        Output_buf[i] = cl::Buffer(context, CL_MEM_WRITE_ONLY, Output_buf_size, NULL, &err);
    }

    unsigned char *Output_Scale[FRAMES], 
                  *Input_Differentiate[FRAMES], *Output_Differentiate[FRAMES];
                //   *Input_Compress[FRAMES];
    int Result_size;
    unsigned char *Input_Scale = (unsigned char*)malloc(INPUT_FRAME_WIDTH * INPUT_FRAME_HEIGHT * FRAMES);
    unsigned char *Output_Compress = (unsigned char*)malloc(MAX_OUTPUT_SIZE);


    for (int i = 0; i < FRAMES; i++){
        Output_Scale[i] = (unsigned char*)malloc(SCALED_FRAME_WIDTH * SCALED_FRAME_HEIGHT);
        Input_Differentiate[i] = (unsigned char*)malloc(OUTPUT_FRAME_WIDTH * OUTPUT_FRAME_HEIGHT);
        Output_Differentiate[i] = (unsigned char*)malloc(OUTPUT_FRAME_WIDTH * OUTPUT_FRAME_HEIGHT);
        if (Output_Scale[i] == NULL){Exit_with_error("malloc failed at main for Output_Scale[i]");}
        if (Input_Differentiate[i] == NULL){Exit_with_error("malloc failed at main for Input_Differentiate[i]");}
        if (Output_Differentiate[i] == NULL){Exit_with_error("malloc failed at main for Output_Differentiate[i]");}
    }

    // for(int i = 0; i < FRAMES; i++)
    // {
    //     Output_Scale[i] = (unsigned char*)q.enqueueMapBuffer(Input_buf[i], CL_TRUE, CL_MAP_WRITE, 0, Input_buf_size);
    //     Input_Differentiate[i] = (unsigned char*)q.enqueueMapBuffer(Output_buf[i], CL_TRUE, CL_MAP_READ, 0, Output_buf_size);
    // }

    timer2.add("Populating input for Scale");
    Load_data(Input_Scale);

    // ------------------------------------------------------------------------------------
    // Step 3: Start computing
    // ------------------------------------------------------------------------------------

    //--- define flags for kernel
    std::vector<cl::Event> write_done(FRAMES);
    std::vector<cl::Event> write_waitlist;
    std::vector<std::vector<cl::Event>> execute_waitlist(FRAMES);
    std::vector<cl::Event> execute_done(FRAMES);
    std::vector<std::vector<cl::Event>> read_waitlist(FRAMES+1);
    std::vector<cl::Event> read_done(FRAMES);

    timer2.add("Running the computation");
    for (int i = 0; i < FRAMES; i++){  //start computation
        printf("before scale: %d\n", i);
        Scale_SW(Input_Scale + i * FRAME_SIZE, Output_Scale[i]);
        printf("after scale: %d\n", i);
        //--------------------------------kernel computation --------------------------------
        krnl_filter.setArg(0, Input_buf[i]);
        krnl_filter.setArg(1, Output_buf[i]);
        printf("before migrate input for filter: %d\n", i);
        if (i == 0){
            q.enqueueMigrateMemObjects({Input_buf[i]}, 0 /* 0 means from host*/, NULL, &write_done[i]);
            write_waitlist.push_back(write_done[i]);
        } else{
            q.enqueueMigrateMemObjects({Input_buf[i]}, 0 /* 0 means from host*/, &write_waitlist, &write_done[i]);
            write_waitlist.push_back(write_done[i]);
        }
        printf("after migrate input for filter: %d\n", i);
        printf("before execute kernel: %d\n", i);
        execute_waitlist[i].push_back(write_done[i]);
        q.enqueueTask(krnl_filter, &execute_waitlist[i], &execute_done[i]);
        printf("before after kernel: %d\n", i);

        printf("before migrate kernel output: %d\n", i);
        read_waitlist[i].push_back(execute_done[i]);
        q.enqueueMigrateMemObjects({Output_buf[i]}, CL_MIGRATE_MEM_OBJECT_HOST, &read_waitlist[i], &read_done[i]);
        read_waitlist[i+1].push_back(read_done[i]);
        printf("after migrate kernel output: %d\n", i);
        //--------------------------------kernel computation --------------------------------
        
/*         cl_int read_status = read_done[i].getInfo<CL_EVENT_COMMAND_EXECUTION_STATUS>();
        printf("Read status: %d\n", read_status);
        if (read_status == CL_COMPLETE) {
            Differentiate_SW(Input_Differentiate[i], Output_Differentiate[i]);
            Result_size = Compress_SW(Output_Differentiate[i], Output_Compress);
        } */

        printf("before doing diff and compress: %d\n", i);
        // read_done[i].wait();
        Differentiate_SW(Input_Differentiate[i], Output_Differentiate[i]);
        Result_size = Compress_SW(Output_Differentiate[i], Output_Compress);
        printf("after doing diff and compress: %d\n", i);
    } //end computation

    q.finish();
    
    // ------------------------------------------------------------------------------------
    // Step 4: Compare Results
    // ------------------------------------------------------------------------------------
    timer2.add("Writing compressed results to output_fpga.bin");
    Store_data("Output.bin", Output_Compress, Result_size);

    timer2.add("Check results with Golden.bin");
    Check_data(Output_Compress, Result_size);
    timer2.finish();
    // ------------------------------------------------------------------------------------
    // Step 5: Release Allocated Resources
    // ------------------------------------------------------------------------------------
    std::cout << "--------------- Key execution times ---------------"
    << std::endl;
    timer2.print();

    timer1.finish();
    std::cout << "--------------- Total time ---------------"
    << std::endl;
    timer1.print();

    for (int i = 0; i < FRAMES; i++){
        free(Output_Scale[i]);
        free(Input_Differentiate[i]);
        free(Output_Differentiate[i]);
    }
    free(Input_Scale);
    free(Output_Compress);

    return 0;
}




