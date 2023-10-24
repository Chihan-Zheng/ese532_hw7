#include "Pipeline.h"
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unistd.h>
#include <fstream>

#define NUM_MAT (5)
#define NUM_TESTS (255)

static void init_arrays(unsigned char *Input[NUM_MAT]) {
  for (int m = 0; m < NUM_MAT; m++) {
      for (int Y = 0; Y < INPUT_FRAME_HEIGHT; Y++) {
        for (int X = 0; X < INPUT_FRAME_WIDTH; X++) {
          Input[m][(Y * INPUT_FRAME_WIDTH) + X] = rand() % (INPUT_FRAME_HEIGHT * INPUT_FRAME_WIDTH);
        }
      }
  }
}

static int result_check(unsigned char *Output[NUM_MAT], unsigned char *Output_sw[NUM_MAT]) {
  for (int m = 0; m < NUM_MAT; m++) {
    for (int i = 0; i < (OUTPUT_FRAME_WIDTH * SCALED_FRAME_HEIGHT); i++) {
      if (Output_sw[m][i] != Output[m][i]) {
        std::cout << "Mismatch: data index=" << i << " dout_sw=" << Output_sw[m][i]
                  << ", dout=" << Output[m][i] << std::endl;
        return 1;
      }
    }
  }
  return 0;
}

int main(int argc, char *argv[]) {
  unsigned char *Input[NUM_MAT], *Output[NUM_MAT], *Output_sw[NUM_MAT];

  for (int m = 0; m < NUM_MAT; m++) {
    Input[m] = (unsigned char *)malloc(INPUT_FRAME_WIDTH * INPUT_FRAME_HEIGHT * sizeof(unsigned char));
    Output[m] = (unsigned char *)malloc(OUTPUT_FRAME_WIDTH * SCALED_FRAME_HEIGHT * sizeof(unsigned char));
    Output_sw[m] = (unsigned char *)malloc(OUTPUT_FRAME_WIDTH * SCALED_FRAME_HEIGHT * sizeof(unsigned char));


    if (!Input[m] || !Output[m] || !Output_sw[m]) {
      if (Input[m])
        free(Input[m]);
      if (Output[m])
        free(Output[m]);
      if (Output_sw[m])
        free(Output_sw[m]);
      return 2;
    }
  }

  init_arrays(Input);
  for (int i = 0; i < NUM_TESTS; i++) {
    Filter_horizontal_SW(Input[i % NUM_MAT], Output_sw[i % NUM_MAT]);
  }

  for (int i = 0; i < NUM_TESTS; i++) {
    Filter_horizontal_HW(Input[i % NUM_MAT], Output[i % NUM_MAT]);
  }

  int failed = 0;
  for (int i = 0; !failed && i < NUM_MAT; i++)
    failed = result_check(Output, Output_sw);
  std::cout << "TEST " << (!failed ? "PASSED" : "FAILED") << std::endl;

  for (int m = 0; m < NUM_MAT; m++) {
    free(Input[m]);
    free(Output[m]);
    free(Output_sw[m]);
  }

  return failed ? 1 : 0;
}
