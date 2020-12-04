#define _POSIX_C_SOURCE 200809L // silly thing to allow use of dprintf() with -std=c11

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdbool.h>            // boolean data type
#include <unistd.h>
#include <getopt.h>             // Not defined by unistd.h for some reason

typedef struct Opts {
    int shift_num;  			// number of shifts
    bool rev;      	 			// shift in reverse
    bool nbr;       			// number the lines of output
} Opts;

typedef struct CC_table {
    char** rot_table;			// will be the cipher lookup table
} CC_table;

// function prototypes
void usage();                   // displays program usage
Opts* handleArgs(int, char**, int*, bool*);  // processes user-supplied option and sets values in Opts struct
CC_table* init_table();         // allocates and populates the lookup table on the heap
void free_table(CC_table*);     // wrapper to free the ilookup table dynamic memory allocation
