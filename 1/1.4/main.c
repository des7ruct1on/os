#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#include "headers/lab.h"

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Your program must start with: %s <FILE_PATH> <FLAG>\n", argv[0]);
        exit(1);
    }
    const char* filename = argv[1];
    const char* flag = argv[2];
    char* mask_hex = NULL;
    FILE* file = fopen(filename, "wb");
    if (!file) {
        printf("Error, can't open file!!!\n");
        exit(1);
    }
    unsigned char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(bytes, sizeof(unsigned char), sizeof(bytes) / sizeof(bytes[0]), file);
    fclose(file);
    int answer;
    status_cmd cmd = command(argv, argc);
    status_code st_xor8, st_xor32, st_mask;
    switch (cmd) {
        case cmd_error_malloc:
            printf("Error malloc detected!\n");
            exit(2);
        case cmd_xor8:
            st_xor8 = xor8(filename, &answer);
            if (st_xor8 != code_error_opening_file) {
                printf("res = %d\n", answer);
                break;
            } else {
                printf("Can`t open file!\n");
                exit(3);
            }
        case cmd_xor32:
            st_xor32 = xor32(filename, &answer);
            if (st_xor32 != code_error_opening_file) {
                printf("res = %d\n", answer);
                break;
            } else {
                printf("Can`t open file!\n");
                exit(3);
            }
        case cmd_mask:
            st_mask = mask(filename, argv[3], &answer);
            if (st_mask != code_error_opening_file) {
                printf("res = %d\n", answer);
                break;
            } else {
                printf("Can`t open file!\n");
                exit(3);
            }
        case cmd_unknown:
            printf("Unknown command!\n");
            break;
        case cmd_error_argc:
            printf("Your programm must start with: %s <FILE_PATH> <MASK> <VALUE>\n", argv[0]);
            break;
    }
    free(mask_hex);
    return 0;
}
