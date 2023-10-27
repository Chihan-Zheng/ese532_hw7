#ifndef CA28A085_6E31_470F_A3D8_5A352ECD2FFC
#define CA28A085_6E31_470F_A3D8_5A352ECD2FFC
#ifndef F2995ADE_7DAE_4116_9B07_0B4B963D88DA
#define F2995ADE_7DAE_4116_9B07_0B4B963D88DA
#ifndef SRC_UTILITIES_
#define SRC_UTILITIES_

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_TARGET_OPENCL_VERSION 120
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_ENABLE_PROGRAM_CONSTRUCTION_FROM_ARRAY_COMPATIBILITY 1
#define CL_USE_DEPRECATED_OPENCL_1_2_APIS

// OCL_CHECK doesn't work if call has templatized function call
#define OCL_CHECK(error, call)                                                 \
  call;                                                                        \
  if (error != CL_SUCCESS) {                                                   \
    printf("%s:%d Error calling " #call ", error code is: %d\n", __FILE__,     \
           __LINE__, error);                                                   \
    exit(EXIT_FAILURE);                                                        \
  }


#include <vector>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <CL/cl2.hpp>
#include "Pipeline.h"
#include "EventTimer.h"

void Exit_with_error(const char *s);
void Load_data(unsigned char *Data);
void Store_data(const char *Filename, unsigned char *Data, unsigned int Size);
void Check_data(unsigned char *Data, unsigned int Size);
std::vector<cl::Device> get_xilinx_devices();
char* read_binary_file(const std::string &xclbin_file_name, unsigned &nb);

#endif


#endif /* F2995ADE_7DAE_4116_9B07_0B4B963D88DA */


#endif /* CA28A085_6E31_470F_A3D8_5A352ECD2FFC */
