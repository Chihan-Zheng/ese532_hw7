#include <iostream>
#include "Pipeline.h"
#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <cstdint>
#include <unistd.h>
#include <vector>

using namespace std;

static void init_arrays(unsigned char *Input) {
	for (int i = 0; i < SCALED_FRAME_HEIGHT; i++) {
		for (int j = 0; j < SCALED_FRAME_WIDTH; j++) {
			Input[i * SCALED_FRAME_WIDTH + j] = rand() % 255;
		}
	}
}

static int check_data(unsigned char *A, unsigned char *B) {
	for (int i = 0; i < OUTPUT_FRAME_HEIGHT; i++) {
		for (int j = 0; j < OUTPUT_FRAME_WIDTH; j++) {
			if (A[i * OUTPUT_FRAME_WIDTH + j] != B[i * OUTPUT_FRAME_WIDTH + j]) {
				cout << (int)A[i * OUTPUT_FRAME_WIDTH + j] << "|" << (int)B[i * OUTPUT_FRAME_WIDTH + j] << endl;
				cout << i << "|" << j << endl;
				return 0;
			}
		}
	}

	return 1;
}

static unsigned char *create_matrix() {
	unsigned char *new_arr = (unsigned char *)malloc((SCALED_FRAME_HEIGHT * SCALED_FRAME_WIDTH) * sizeof(unsigned char));
	if (new_arr == NULL)
		return NULL;
	return new_arr;
}

int main() {

	// Create Matrices.
	unsigned char *Input = create_matrix();
	unsigned char *Output_HW = create_matrix();
	unsigned char *Output_SW = create_matrix();

	// Initialize arrays.
	init_arrays(Input);

	/* Test Filter horizontal.
	Filter_horizontal_SW(Input, Output_SW);
	Filter_horizontal_HW(Input, Output_HW);
	*/

	/*
	Filter_vertical_SW(Input, Output_SW);
	Filter_vertical_HW(Input, Output_HW);
	 */


	Filter_SW(Input, Output_SW);
	Filter_HW(Input, Output_HW);


	if (check_data(Output_HW, Output_SW) == 1)
		cout << "TEST PASSED!" << endl;
	else {
		cout << "TEST FAILED!" << endl;
		free(Input);
		free(Output_SW);
		free(Output_HW);
		return 1;
	}

	free(Input);
	free(Output_SW);
	free(Output_HW);

	return EXIT_SUCCESS;
}
