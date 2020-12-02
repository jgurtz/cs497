#define _POSIX_C_SOURCE 200809L // silly thing to allow use of dprintf()

#include <getopt.h>             // Not defined by unistd.h for some reason

#include "ccipher.h"
#include "cc_table.h"           // Custom data type having lookup table


void usage(int ec) {
    printf("Name:\n");
    printf("    ccipher\n\n");
    printf("Synopsis:\n");
    printf("    ccipher -s NUM [-r] [-n] [FILE] ...\n\n");
    printf("Description:\n");
    printf("    Caeser cipher the contents of FILE, then concatenate to stdout.\n\n");
    printf("    If FILE is `-' (a hyphen), operate on stdin. Remaining files ignored.\n\n");
    printf("Options:\n");
    printf("    -s NUM  Shift letters NUM times. NUM is from 1 to 13\n\n");
    printf("    -r      Reverse, or decipher.\n\n");
    printf("    -n      Number all output lines.\n\n");
    printf("    -h      Show this help.\n\n");

    exit(ec);
}

Opts* handleArgs(int ac, char** av) {
    int c;
    static Opts op = {.shift_num=0, .rev=false, .nbr=false};

    (ac == 1) ? usage(0) : true;

    while ( (c = getopt(ac, av, "rnhs:") ) != -1) {
        switch(c) {
            case 's':
                op.shift_num = atoi(optarg);
                break;
            case 'r':
                op.rev = true;
                break;
            case 'n':
                op.nbr = true;
                break;
            case 'h':
            default:
                usage(0);
        }
    }

    if (op.shift_num < 1 ||
        op.shift_num > 13) {
        usage(255);
    }
    return &op;
}

int main(int argc, char** argv) {
    //CC_table* rots;
    CC_table* rots = init_table();          // Generates lookup table on the heap; see cc_table.h
    const char *infile;                     // input filename
    char buf;                               // char buffer
    int argidx;                             // saves curr. index of argv after getopt()
    int fd_in;                              // input file descriptor
    int bc_read = 0;                        // Byte read counter
    int char_idx = 0;                       // Index num in CC_TABLE[0]

    Opts* opts = handleArgs(argc, argv);    // Parses all cli options until the files

    //DEBUG
    //printf("After --> SN: %d, RV: %d, NR: %d\n", opts->shift_num, opts->rev, opts->nbr);
    //printf("First letter: %c\n", CC_TABLE[opts->shift_num][ strcspn(CC_TABLE[0], "A") ]);
    //printf("Length of CC_TABLE[0]: %lu\n", strlen(CC_TABLE[0]));

    //infile = "-";   // default to reading from stdin
    argidx = optind;

    do {
        if (argidx < argc) {
            infile = argv[argidx];

            if ( 0 == strncmp(infile, "-", strlen(infile)) ) {
                fd_in = STDIN_FILENO;
            }
            else {
                fd_in = open(infile, O_RDONLY);
            }

            if (fd_in < 0) {
                perror( strncat("Error opening file", infile, strlen(infile)) );
                exit(errno);
            }

            while ( (bc_read = read(fd_in, &buf, 1)) > 0 ) {

                switch ( (int)opts->rev ) {
                    case 0: // false
                        char_idx = strcspn(rots->rot_table[0], &buf);   // find index of char in alphabet
                        (char_idx == 52) ? printf("%c", buf) : printf("%c", rots->rot_table[opts->shift_num][char_idx]);
                        break;
                    case 1: // true
                        char_idx = strcspn(rots->rot_table[opts->shift_num], &buf);   // find index of char in alphabet
                        (char_idx == 52) ? printf("%c", buf) : printf("%c", rots->rot_table[0][char_idx]);
                }
                //DEBUG
                //printf("char_idx: %d\n", char_idx);

            }

            if (bc_read < 0) {
                dprintf( STDERR_FILENO, "Error occured reading from file %s: %s\n",
                    infile, strerror(errno) );

                close(fd_in);
                exit(errno);
            }
        }
    }
    while (++argidx < argc);

    free_table(rots);   // deallocate the heap memory

    return 0;
}
// vim: expandtab:smartindent:ts=4:sw=4
