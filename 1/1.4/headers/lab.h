#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>

typedef enum status_code {
    code_success,
    code_error_opening_file
} status_code;

typedef enum status_cmd {
    cmd_error_argc,
    cmd_error_malloc,
    cmd_xor8,
    cmd_xor32,
    cmd_mask,
    cmd_unknown
} status_cmd;

status_code xor8(const char* file_path, int* answer);
status_code xor32(const char* file_path, int* answer);
status_code mask(const char* file_path, char* mask_hex, int* answer);
status_cmd command(char* argv[], int argc);

#endif