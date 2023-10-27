#include "Pipeline.h"
#include <stdlib.h>
#include <hls_stream.h>
#include <stdio.h>

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
		                      hls::stream<unsigned char>& outStream)
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

      // Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;   //original version
      outStream.write(Sum >> 8);
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


void Filter_vertical_HW(hls::stream<unsigned char>& inStream,
		                    unsigned char * Output)
{
  int X, Y, i, j;
  const char INPUT_BUFFER_LENGTH = 7;       

  unsigned char Coefficients_local[FILTER_LENGTH];
  unsigned char Input_local[OUTPUT_FRAME_WIDTH][INPUT_BUFFER_LENGTH];

  #pragma HLS ARRAY_PARTITION variable=Coefficients_local complete 
  #pragma HLS ARRAY_PARTITION variable=Input_local cyclic factor=7 dim=2 

  for (i = 0; i < FILTER_LENGTH; i++) {Coefficients_local[i] = Coefficients[i];}
  for (j = 0; j < INPUT_BUFFER_LENGTH; j++) {
    for (i = 0; i < OUTPUT_FRAME_WIDTH; i++){
      Input_local[i][j] = inStream.read();
    }
  }

  for (Y = 0; Y < OUTPUT_FRAME_HEIGHT; Y++){
    for (X = 0; X < OUTPUT_FRAME_WIDTH; X++)
    {
      #pragma HLS PIPELINE
      unsigned int Sum = 0;

      for (i = 0; i < FILTER_LENGTH; i++){  
        #pragma HLS unroll
        // Sum += Coefficients[i] * Input[OUTPUT_FRAME_WIDTH * (Y+i) + X];   //SW version
        Sum += Coefficients_local[i] * Input_local[X][i];
      }
      Output[Y * OUTPUT_FRAME_WIDTH + X] = Sum >> 8;
    }

    if (Y == (OUTPUT_FRAME_HEIGHT - 1)) {return;}
    for (i = 0; i < OUTPUT_FRAME_WIDTH; i++){
      #pragma HLS PIPELINE
      for (j = 0; j < INPUT_BUFFER_LENGTH - 1; j++){
        #pragma HLS unroll
        Input_local[i][j] = Input_local[i][j+1];
      } 
      Input_local[i][INPUT_BUFFER_LENGTH - 1] = inStream.read();
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
  static hls::stream<unsigned char> tempStream("Temp");
  #pragma HLS STREAM variable=tempStream
  #pragma HLS dataflow
  #pragma HLS INTERFACE m_axi port=Input bundle=aximm1
  #pragma HLS INTERFACE m_axi port=Output bundle=aximm2
  Filter_horizontal_HW(Input, tempStream);
  Filter_vertical_HW(tempStream, Output);

}
