#include <stdlib.h>

typedef struct CC_table {
    // [14][53];
    char** rot_table;
} CC_table;

void free_table(CC_table* tbl) {

    free(tbl->rot_table);
    free(tbl);
}

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
