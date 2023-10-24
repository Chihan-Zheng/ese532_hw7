#include "Pipeline.h"
#include <stdlib.h>
#include <iostream>
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
  int INPUT_BUFFER_LENGTH = 7;

  #ifdef NO_SYNTH
    unsigned int *Coefficients_local = (unsigned int*) malloc(FILTER_LENGTH * sizeof(unsigned int));
    unsigned char *Input_local = (unsigned char*) malloc(INPUT_BUFFER_LENGTH * sizeof(unsigned char));
  #else
    unsigned int _Coefficients_local[FILTER_LENGTH];
    unsigned char _Input_local[INPUT_BUFFER_LENGTH];
    unsigned int *Coefficients_local = &(_Coefficients_local[0]);
    unsigned char *Input_local = &(_Input_local[0]);
  #endif

  for (i = 0; i < FILTER_LENGTH; i++) {Coefficients_local[i] = Coefficients[i];}

  for (Y = 0; Y < SCALED_FRAME_HEIGHT; Y++){
    for (i = 1; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[(Y * SCALED_FRAME_WIDTH) + i - 1];}
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
      unsigned int Sum = 0;
      for (i = 0; i < (INPUT_BUFFER_LENGTH - 1); i++) {Input_local[i] = Input_local[i+1];}
      Input_local[INPUT_BUFFER_LENGTH - 1] = Input[(Y * SCALED_FRAME_WIDTH) + X + FILTER_LENGTH - 1];
      for (i = 0; i < FILTER_LENGTH; i++){
#pragma HLS PIPELINE
        // Sum += Coefficients[i] * Input[Y * SCALED_FRAME_WIDTH + X + i];   //SW version
        Sum += Coefficients_local[i] * Input_local[i];
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


static void Filter_vertical_HW(const unsigned char * Input,
		                    unsigned char * Output)
{
  int X, Y, i;
  int INPUT_BUFFER_LENGTH = 7;

  #ifdef NO_SYNTH
    unsigned int *Coefficients_local = (unsigned int*) malloc(FILTER_LENGTH * sizeof(unsigned int));
    unsigned char *Input_local = (unsigned char*) malloc(INPUT_BUFFER_LENGTH * sizeof(unsigned char));
  #else
    unsigned int _Coefficients_local[FILTER_LENGTH];
    unsigned char _Input_local[INPUT_BUFFER_LENGTH];
    unsigned int *Coefficients_local = &(_Coefficients_local[0]);
    unsigned char *Input_local = &(_Input_local[0]);
  #endif

  for (i = 0; i < FILTER_LENGTH; i++) {Coefficients_local[i] = Coefficients[i];}

  for (X = 0; X < OUTPUT_FRAME_HEIGHT; X++){
    for (i = 1; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[X + OUTPUT_FRAME_WIDTH * (i-1)];}
    for (Y = 0; Y < OUTPUT_FRAME_WIDTH; Y++)
    {
      unsigned int Sum = 0;
      for (i = 0; i < (INPUT_BUFFER_LENGTH - 1); i++) {Input_local[i] = Input_local[i+1];}
      Input_local[INPUT_BUFFER_LENGTH - 1] = Input[X + OUTPUT_FRAME_WIDTH * (FILTER_LENGTH-1)];
      for (i = 0; i < FILTER_LENGTH; i++){
        // Sum += Coefficients[i] * Input[OUTPUT_FRAME_WIDTH * i + X];   //SW version
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
