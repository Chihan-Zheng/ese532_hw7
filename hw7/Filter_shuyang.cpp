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
  int INPUT_BUFFER_LENGTH = 6;

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
    for (i = 0; i < INPUT_BUFFER_LENGTH; i++) {Input_local[i] = Input[(Y * SCALED_FRAME_WIDTH) + i];}
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
      unsigned int Sum = 0;

      for (i = 0; i < FILTER_LENGTH; i++){
        if(i==FILTER_LENGTH-1){
          for (int j = 0; j < (INPUT_BUFFER_LENGTH - 1); j++) {Input_local[j] = Input_local[j+1];}
          Input_local[INPUT_BUFFER_LENGTH - 1] = Input[(Y * SCALED_FRAME_WIDTH) + X + i];
        }
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

void Filter_SW(const unsigned char * Input,
	           unsigned char * Output)
{
  unsigned char * Temp = (unsigned char *) malloc(SCALED_FRAME_HEIGHT * OUTPUT_FRAME_WIDTH);
  Filter_horizontal_SW(Input, Temp);
  Filter_vertical_SW(Temp, Output);
  free(Temp);
}
