#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <stdbool.h>


typedef struct Options {
    int shift_num;  // number of shifts
    bool rev;       // shift in reverse
    bool nbr;       // number the lines of output
} Opts;

void usage();
Opts* handleArgs(int, char**);
