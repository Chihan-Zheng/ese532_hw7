#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS
#define STAGES (4)

#include <CL/cl2.hpp>
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "Pipeline.h"
#include "Utilities.h"
#include "Stopwatch.h"
#include "EventTimer.h"

int main(int argc, char *argv[])
{
    unsigned char *Input_data = (unsigned char *)malloc(FRAMES * FRAME_SIZE);
    unsigned char *Temp_data[STAGES - 1];
    unsigned char *Output_data = (unsigned char *)malloc(MAX_OUTPUT_SIZE);
    for (int Stage = 0; Stage < STAGES - 1; Stage++){
        Temp_data[Stage] = (unsigned char *)malloc(FRAME_SIZE);
        if (Temp_data[Stage] == NULL)
            Exit_with_error("malloc failed at main for Temp_data");
    }
    std::cout << "Loading Input.bin" << std::endl;
    Load_data(Input_data);
    stopwatch time_scale;
    stopwatch time_filter;
    stopwatch time_differentiate;
    stopwatch time_compress;
    stopwatch total_time;
    int Size = 0;

    // Step 1: Initialize the OpenCL environment
    std::cout << "Initializing OpenCL environment" << std::endl;
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
    cl::Kernel Filter_HW(program, "Filter_HW", &err);

    // Step 2: Create OpenCL buffers and map them to host buffers
    std::cout << "Initializing OpenCL buffers" << std::endl;
    size_t elements_per_iteration = FRAME_SIZE;
    size_t bytes_per_iteration = elements_per_iteration * sizeof(unsigned char);
    cl::Buffer Temp_0_buf;
    cl::Buffer Temp_1_buf;
    Temp_0_buf = cl::Buffer(context, CL_MEM_READ_ONLY, bytes_per_iteration, NULL, &err);
    Temp_1_buf = cl::Buffer(context, CL_MEM_WRITE_ONLY, bytes_per_iteration, NULL, &err);
    Temp_data[0] = (unsigned char*)q.enqueueMapBuffer(Temp_0_buf, CL_TRUE, CL_MAP_WRITE, 0, bytes_per_iteration);
    Temp_data[1] = (unsigned char*)q.enqueueMapBuffer(Temp_1_buf, CL_TRUE, CL_MAP_READ, 0, bytes_per_iteration);

    // Step 3: Run the compression flow on host + kernel
    td::cout << "Compressing " << FRAMES << " frames" << std::endl;
    std::vector<cl::Event> write_events, exec_events, read_events;
    cl::Event write_ev, exec_ev, read_ev;
    for (int Frame = 0; Frame < FRAMES; Frame++){
        total_time.start();
        //Scale on host
        time_scale.start();
        Scale_SW(Input_data + Frame * FRAME_SIZE, Temp_data[0]);
        time_scale.stop();
        //Filter on FPGA kernel
        time_filter.start();
        Filter_HW.setArg(0, Temp_0_buf);
        Filter_HW.setArg(1, Temp_1_buf);
        q.enqueueMigrateMemObjects({Temp_0_buf}, 0 /* 0 means from host*/, NULL, &write_ev);
        write_events.push_back(write_ev);
        q.enqueueTask(Filter_HW, &write_events, &exec_ev);
        exec_events.push_back(exec_ev);
        write_events.pop_back();
        q.enqueueMigrateMemObjects({Temp_1_buf}, CL_MIGRATE_MEM_OBJECT_HOST, &exec_events, &read_ev);
        read_events.push_back(read_ev);
        exec_events.pop_back();
        read_events[0].wait();
        read_events.pop_back();
        time_filter.stop();
        //Differentiate on host
        time_differentiate.start();
        Differentiate_SW(Temp_data[1], Temp_data[2]);
        time_differentiate.stop();
        //Compression on host
        time_compress.start();
        Size = Compress_SW(Temp_data[2], Output_data);
        time_compress.stop();
        total_time.stop();
    }
    q.enqueueUnmapMemObject(Temp_0_buf, Temp_data[0]);
    q.enqueueUnmapMemObject(Temp_1_buf, Temp_data[1]);
    q.finish();
    std::cout << "Total latency of Scale is: " << time_scale.latency() << " ns." << std::endl;
    std::cout << "Total latency of Filter is: " << time_filter.latency() << " ns." << std::endl;
    std::cout << "Total latency of Differentiate is: " << time_differentiate.latency() << "ns." << std::endl;
    std::cout << "Total latency of Compress is: " << time_compress.latency() << " ns." << std::endl;
    std::cout << "Total time taken by the loop is: " << total_time.latency() << " ns." << std::endl;
    std::cout << "---------------------------------------------------------------" << std::endl;
    std::cout << "Average latency of Scale per loop iteration is: " << time_scale.avg_latency() << " ns." << std::endl;
    std::cout << "Average latency of Filter per loop iteration is: " << time_filter.avg_latency() << " ns." << std::endl;
    std::cout << "Average latency of Differentiate per loop iteration is: " << time_differentiate.avg_latency() << " ns." << std::endl;
    std::cout << "Average latency of Compress per loop iteration is: " << time_compress.avg_latency() << " ns." << std::endl;
    std::cout << "Average latency of each loop iteration is: " << total_time.avg_latency() << " ns." << std::endl;
    Store_data("Output.bin", Output_data, Size);
    Check_data(Output_data, Size);

    for (int i = 2; i < STAGES - 1; i++)
        free(Temp_data[i]);
    free(Input_data);
    free(Output_data);
    return EXIT_SUCCESS;
    }