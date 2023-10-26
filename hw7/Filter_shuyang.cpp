#include "Pipeline.h"
#include <stdlib.h>
#include <iostream>
#define NO_SYNTH

static unsigned Coefficients[] = {2, 15, 62, 98, 62, 15, 2};

void Filter_horizontal_SW(const unsigned char * Input,
		                      unsigned char * Output)
{
  int X, Y, i;
  for (Y = 0; Y < SCALED_FRAME_HEIGHT; Y++)
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
      unsigned int Sum = 0;
      for (i = 0; i < FILTER_LENGTH; i++)
        Sum += Coefficients[i] * Input[Y * SCALED_FRAME_WIDTH + X + i];
      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }
}


void Filter_horizontal_HW(const unsigned char * Input,
		                      unsigned char * Output)
{
  int X, Y, i;
  const char INPUT_BUFFER_LENGTH = 6;       

  #ifdef NO_SYNTH
    unsigned char *Coefficients_local = (unsigned char*) malloc(FILTER_LENGTH * sizeof(unsigned char));
    unsigned char *Input_local = (unsigned char*) malloc(INPUT_BUFFER_LENGTH * sizeof(unsigned char));
  #else
    unsigned char Coefficients_local[FILTER_LENGTH];
    unsigned char Input_local[INPUT_BUFFER_LENGTH];
  #endif

#pragma HLS ARRAY_PARTITION variable=Coefficients_local complete 
#pragma HLS ARRAY_PARTITION variable=Input_local complete 

  for (i = 0; i < FILTER_LENGTH; i++) {Coefficients_local[i] = Coefficients[i];}

  for (Y = 0; Y < SCALED_FRAME_HEIGHT; Y++){
    for (i = 0; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[(Y * SCALED_FRAME_WIDTH) + i];}
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
#pragma HLS PIPELINE
      unsigned int Sum = 0;

      Input_local[INPUT_BUFFER_LENGTH - 1] = Input[(Y * SCALED_FRAME_WIDTH) + X + FILTER_LENGTH - 1];
      for (i = 0; i < FILTER_LENGTH; i++){
#pragma HLS unroll
        if(i==FILTER_LENGTH-1){
          for (i = 0; i < (INPUT_BUFFER_LENGTH - 1); i++) {
#pragma HLS unroll
            Input_local[i] = Input_local[i+1];
          }
          Input_local[INPUT_BUFFER_LENGTH - 1] = Input[(Y * SCALED_FRAME_WIDTH) + X + FILTER_LENGTH - 1];
          Sum += Coefficients_local[i] * Input_local[INPUT_BUFFER_LENGTH - 1];
        }
        // Sum += Coefficients[i] * Input[Y * SCALED_FRAME_WIDTH + X + i];   //SW version
        else{
          Sum += Coefficients_local[i] * Input_local[i];
        } 
      }
      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }
  }
}


static void Filter_vertical_SW(const unsigned char * Input,
		                    unsigned char * Output)
{
  int X, Y, i;
  for (Y = 0; Y < OUTPUT_FRAME_HEIGHT; Y++)
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
      unsigned int Sum = 0;
      for (i = 0; i < FILTER_LENGTH; i++)
        Sum += Coefficients[i] * Input[(Y + i) * OUTPUT_FRAME_WIDTH + X];
      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }
}

void Filter_SW(const unsigned char * Input,
	           unsigned char * Output)
{
  unsigned char * Temp = (unsigned char *) malloc(SCALED_FRAME_HEIGHT * OUTPUT_FRAME_WIDTH);
  Filter_horizontal_SW(Input, Temp);
  Filter_vertical_SW(Temp, Output);
  free(Temp);
}
