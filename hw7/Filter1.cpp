#include "Pipeline.h"
#include <stdlib.h>
#include <hls_stream.h>

// #define NO_SYNTH      //define when need to do Csim and comment out when need to do synthesis

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
  const char INPUT_BUFFER_LENGTH = 7;       

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
    for (i = 1; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[(Y * SCALED_FRAME_WIDTH) + i - 1];}
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
#pragma HLS PIPELINE
      unsigned int Sum = 0;
      for (i = 0; i < (INPUT_BUFFER_LENGTH - 1); i++) {
#pragma HLS unroll
        Input_local[i] = Input_local[i+1];
        }

      Input_local[INPUT_BUFFER_LENGTH - 1] = Input[(Y * SCALED_FRAME_WIDTH) + X + FILTER_LENGTH - 1];
      for (i = 0; i < FILTER_LENGTH; i++){
#pragma HLS unroll
        // Sum += Coefficients[i] * Input[Y * SCALED_FRAME_WIDTH + X + i];   //SW version
        Sum += Coefficients_local[i] * Input_local[i];
      }

      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }
  }
}


void Filter_vertical_SW(const unsigned char * Input,
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


void Filter_vertical_HW(const unsigned char * Input,
		                    unsigned char * Output)
{
  int X, Y, i;
  const char INPUT_BUFFER_LENGTH = 7;       

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

  for (X = 0; X < OUTPUT_FRAME_WIDTH; X++){
    for (i = 1; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[X + OUTPUT_FRAME_WIDTH * (i-1)];}
    for (Y = 0; Y < OUTPUT_FRAME_HEIGHT; Y++)
    {
#pragma HLS PIPELINE
      unsigned int Sum = 0;
      for (i = 0; i < (INPUT_BUFFER_LENGTH - 1); i++) {
#pragma HLS unroll
        Input_local[i] = Input_local[i+1];
      }

      Input_local[INPUT_BUFFER_LENGTH - 1] = Input[X + OUTPUT_FRAME_WIDTH * (Y + FILTER_LENGTH-1)];
      for (i = 0; i < FILTER_LENGTH; i++){  
#pragma HLS unroll
        // Sum += Coefficients[i] * Input[OUTPUT_FRAME_WIDTH * (Y+i) + X];   //SW version
        Sum += Coefficients_local[i] * Input_local[i];
      }

      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }
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

void Filter_HW(const unsigned char * Input,
	           unsigned char * Output)
{
  #ifdef NO_SYNTH
    unsigned char *Temp = (unsigned char *) malloc(SCALED_FRAME_HEIGHT * OUTPUT_FRAME_WIDTH);
  #else
    unsigned char Temp[SCALED_FRAME_HEIGHT * OUTPUT_FRAME_WIDTH];
  #endif

  Filter_horizontal_HW(Input, Temp);
  Filter_vertical_HW(Temp, Output);

  #ifdef NO_SYNTH
    free(Temp);
  #endif

}
