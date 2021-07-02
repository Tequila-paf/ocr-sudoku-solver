#ifndef FILE_HANDLING_H_
#define FILE_HANDLING_H_

#include <stdio.h>
#include <stdlib.h>

//#include "solver_tools.h"

void read_file(char *name, int grid[9][9]);

void write_file(char *name, int grid[9][9]);

size_t my_str_len(char str[]);

#endif
