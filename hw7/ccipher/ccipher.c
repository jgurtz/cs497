#include "ccipher.h"


void usage(int ec) {
    printf("Name:\n");
    printf("    ccipher\n\n");
    printf("Synopsis:\n");
    printf("    ccipher -s NUM [-r] [-n] FILE ...\n\n");
    printf("Description:\n");
    printf("    Caeser cipher the contents of FILE, then concatenate to stdout.\n\n");
    printf("    If FILE is `-' (a hyphen), operate on stdin. Remaining files ignored.\n\n");
    printf("Options:\n");
    printf("    -s NUM  Shift letters NUM times. NUM is from 0 to 13\n\n");
    printf("    -r      Reverse, or decipher.\n\n");
    printf("    -n      Number all output lines.\n\n");
    printf("    -h      Show this help.\n\n");

    exit(ec);
}

Opts* handleArgs(int ac, char** av, int* ln, bool* pl) {
    int c;
    static Opts op = {.shift_num=0, .rev=false, .nbr=false};

    (ac < 4) ? usage(253) : true;

    while ( (c = getopt(ac, av, "rnhs:") ) != -1) {
        switch(c) {
            case 's':
                op.shift_num = atoi(optarg);
                break;
            case 'r':
                op.rev = true;
                break;
            case 'n':
                op.nbr = true; *ln = 1; *pl = true;
                break;
            case 'h':
            default:
                usage(255);
        }
    }

    if (op.shift_num < 0 ||
        op.shift_num > 13) {
        usage(254);
    }
    return &op;
}

// Cipher operations are done via this table
CC_table* init_table() {
    CC_table* r = malloc(sizeof(*r));
    r->rot_table = malloc( 14 * sizeof(*r->rot_table) );

    // trailing nulls needed to allow index seaerching with strcspn()
    r->rot_table[0]  = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz\0";
    r->rot_table[1]  = "BCDEFGHIJKLMNOPQRSTUVWXYZAbcdefghijklmnopqrstuvwxyza\0";
    r->rot_table[2]  = "CDEFGHIJKLMNOPQRSTUVWXYZABcdefghijklmnopqrstuvwxyzab\0";
    r->rot_table[3]  = "DEFGHIJKLMNOPQRSTUVWXYZABCdefghijklmnopqrstuvwxyzabc\0";
    r->rot_table[4]  = "EFGHIJKLMNOPQRSTUVWXYZABCDefghijklmnopqrstuvwxyzabcd\0";
    r->rot_table[5]  = "FGHIJKLMNOPQRSTUVWXYZABCDEfghijklmnopqrstuvwxyzabcde\0";
    r->rot_table[6]  = "GHIJKLMNOPQRSTUVWXYZABCDEFghijklmnopqrstuvwxyzabcdef\0";
    r->rot_table[7]  = "HIJKLMNOPQRSTUVWXYZABCDEFGhijklmnopqrstuvwxyzabcdefg\0";
    r->rot_table[8]  = "IJKLMNOPQRSTUVWXYZABCDEFGHijklmnopqrstuvwxyzabcdefgh\0";
    r->rot_table[9]  = "JKLMNOPQRSTUVWXYZABCDEFGHIjklmnopqrstuvwxyzabcdefghi\0";
    r->rot_table[10] = "KLMNOPQRSTUVWXYZABCDEFGHIJklmnopqrstuvwxyzabcdefghij\0";
    r->rot_table[11] = "LMNOPQRSTUVWXYZABCDEFGHIJKlmnopqrstuvwxyzabcdefghijk\0";
    r->rot_table[12] = "MNOPQRSTUVWXYZABCDEFGHIJKLmnopqrstuvwxyzabcdefghijkl\0";
    r->rot_table[13] = "NOPQRSTUVWXYZABCDEFGHIJKLMnopqrstuvwxyzabcdefghijklm\0";

    return r;
}

void free_table(CC_table* tbl) {
    free(tbl->rot_table);
    free(tbl);
}

int main(int argc, char** argv) {
    CC_table* rots = init_table();          // Generates lookup table on the heap
    const char *infile;                     // input filename
    char buf;                               // char buffer
    int argidx;                             // saves curr. index of argv after getopt()
    int fd_in;                              // input file descriptor
    int bc_read = 0;                        // Byte read counter
    int char_idx = 0;                       // Index num in CC_TABLE[0]
    int line_num = 0;                       // Holds current line number for output
    bool print_line = false;                // keep track if line number needs printing

    Opts* opts = handleArgs(argc, argv, &line_num, &print_line);  // Parse all cli options until input files
    argidx = optind;    // idx to remaining argv after processing the flags

    do {
        if (argidx < argc) {
            infile = argv[argidx];

            if ( 0 == strncmp(infile, "-", strlen(infile)) ) {
                fd_in = STDIN_FILENO;
                argidx = argc;  // no more files after reading from stdin
            }
            else {
                fd_in = open(infile, O_RDONLY);
                if (fd_in < 0) {
                    perror( strcat("Error opening file: ", infile) );
                    exit(errno);
                }
            }

            while ( (bc_read = read(fd_in, &buf, 1)) > 0 ) {
                if (bc_read < 0) {
                    dprintf( STDERR_FILENO, "Error occured reading from file %s: %s\n", infile, strerror(errno) );
                    close(fd_in);
                    exit(errno);
                }

                if (print_line == true) {
                    printf("%11d: ", line_num); // aligns correctly up to 999,999,999 lines
                    print_line = false;
                }

                switch ( (int)opts->rev ) {
                    // char_idx of 52 means char was not found in [A-Za-z]
                    case 0: // false, rotate forward
                        char_idx = strcspn(rots->rot_table[0], &buf);

                        if (char_idx == 52) {
                            // NOT an alpha char
                            // detect end of line and line numbering feature
                            if (buf == '\n' && opts->nbr == true) {
                                 line_num++; print_line = true;
                            }
                            printf("%c", buf);
                        }
                        else {
                            // IS an alpha char
                            printf("%c", rots->rot_table[opts->shift_num][char_idx]);
                        }
                        break;
                    case 1: // true, rotate back
                        char_idx = strcspn(rots->rot_table[opts->shift_num], &buf);
                        //(char_idx == 52) ? printf("%c", buf) : printf("%c", rots->rot_table[0][char_idx]);
                        if (char_idx == 52) {
                            // NOT an alpha char
                            // detect end of line and line numbering feature
                            if (buf == '\n' && opts->nbr == true) {
                                 line_num++; print_line = true;
                            }
                            printf("%c", buf);
                        }
                        else {
                            printf("%c", rots->rot_table[0][char_idx]);
                        }
                }
            }
            close(fd_in);
        }
    } while (++argidx < argc);

    free_table(rots);   // deallocate the heap memory

    return 0;
}
// vim: expandtab:smartindent:ts=4:sw=4
