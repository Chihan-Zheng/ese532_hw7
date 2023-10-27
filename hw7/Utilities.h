#ifndef SRC_UTILITIES_
#define SRC_UTILITIES_

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <thread>
#include <vector>
#include "Pipeline.h"

void Exit_with_error(const char *s);
void Load_data(unsigned char *Data);
void Store_data(const char *Filename, unsigned char *Data, unsigned int Size);
void Check_data(unsigned char *Data, unsigned int Size);

#endif
