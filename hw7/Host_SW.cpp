
#include <cstdint>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>

#include "Utilities.h"

int main()
{

    unsigned char *Output_Scale, 
                  *Input_Differentiate, *Output_Differentiate;
                //   *Input_Compress[FRAMES];
    // unsigned char *Output_Scale[FRAMES], 
    //               *Input_Differentiate[FRAMES], *Output_Differentiate[FRAMES];
    //             //   *Input_Compress[FRAMES];
    int Result_size=0;
    unsigned char *Input_Scale = (unsigned char*)malloc(INPUT_FRAME_WIDTH * INPUT_FRAME_HEIGHT * FRAMES);
    unsigned char *Output_Compress = (unsigned char*)malloc(MAX_OUTPUT_SIZE*100);

    
    Output_Scale = (unsigned char*)malloc(FRAME_SIZE);
    Input_Differentiate = (unsigned char*)malloc(FRAME_SIZE);
    Output_Differentiate = (unsigned char*)malloc(FRAME_SIZE);
    if (Output_Scale == NULL){Exit_with_error("malloc failed at main for Output_Scale[i]");}
    if (Input_Differentiate == NULL){Exit_with_error("malloc failed at main for Input_Differentiate[i]");}
    if (Output_Differentiate == NULL){Exit_with_error("malloc failed at main for Output_Differentiate[i]");}

    Load_data(Input_Scale);

    for (int i = 0; i < FRAMES; i++){  //start computation
        printf("before scale: %d\n", i);
        Scale_SW(Input_Scale + i * FRAME_SIZE, Output_Scale);
        printf("after scale: %d\n", i);

        Filter_SW(Output_Scale, Input_Differentiate);

        printf("before doing diff and compress: %d\n", i);
        // read_done[i].wait();
        Differentiate_SW(Input_Differentiate, Output_Differentiate);
        Result_size = Compress_SW(Output_Differentiate, Output_Compress);
        printf("after doing diff and compress: %d\n", i);
    } //end computation
    

    
    // ------------------------------------------------------------------------------------
    // Step 4: Compare Results
    // ------------------------------------------------------------------------------------
   
    Store_data("Output.bin", Output_Compress, Result_size);
    printf("Result_size: %d\n", Result_size);
    Check_data(Output_Compress, Result_size);
    printf("reach free\n");
    free(Output_Scale);
    free(Input_Differentiate);
    free(Output_Differentiate);
    
    free(Input_Scale);
    free(Output_Compress);

    return 0;
}




