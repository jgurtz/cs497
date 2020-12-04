#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>

#include "cc_table.h"   // custom lookup table implementation

typedef struct Options {
    int shift_num;  // number of shifts
    bool rev;       // shift in reverse
    bool nbr;       // number the lines of output
} Opts;

// function prototypes
void usage();                   // displays program usage
Opts* handleArgs(int, char**);  // processes user-supplied option and sets values in Options struct
void free_table(CC_table*);     // wrapper to free the dynamic memory allocation
CC_table* init_table();         // allocates and populates the lookup table on the heap
