#define _POSIX_C_SOURCE 200809L // silly thing to allow use of dprintf()

#include <getopt.h>             // Not defined by unistd.h for some reason

#include "ccipher.h"


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
    int line_num = 0;                       // Holds current line number for output
    bool print_line = false;                // keep track if line number needs printing

    Opts* opts = handleArgs(argc, argv);    // Parses all cli options until the files

    argidx = optind;    // idx to remaining argv after processing the flags
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

            if (opts->nbr == true) {
                line_num = 1;
                print_line = true;
            }

            while ( (bc_read = read(fd_in, &buf, 1)) > 0 ) {

                switch ( (int)opts->rev ) {
                    // char_idx of 52 means char was not found in [A-Za-z]
                    case 0: // false
                        char_idx = strcspn(rots->rot_table[0], &buf);   // find index of char in alphabet
                        if (char_idx == 52) {
                            //not an alpha char

                            if (print_line == true) {
                                printf("%11d: ", line_num); // aligns correctly up to 999,999,999 lines
                                print_line = false;
                            }

                            if (buf == '\n' && opts->nbr == true) {
                                 line_num++;
                                 print_line = true;
                            }
                            printf("%c", buf);
                        }
                        else {
                            // is alpha char
                            if (print_line == true) {
                                printf("%11d: ", line_num);
                                print_line = false;
                            }

                            printf("%c", rots->rot_table[opts->shift_num][char_idx]);
                        }
                        break;
                    case 1: // true
                        char_idx = strcspn(rots->rot_table[opts->shift_num], &buf);   // find index of char in alphabet
                        (char_idx == 52) ? printf("%c", buf) : printf("%c", rots->rot_table[0][char_idx]);
                }
            }

            if (bc_read < 0) {
                dprintf( STDERR_FILENO, "Error occured reading from file %s: %s\n",
                    infile, strerror(errno) );

                close(fd_in);
                exit(errno);
            }
            close(fd_in);
        }
    } while (++argidx < argc);

    free_table(rots);   // deallocate the heap memory

    return 0;
}
// vim: expandtab:smartindent:ts=4:sw=4
