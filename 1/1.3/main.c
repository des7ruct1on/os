#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#define STR_SIZE 256

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Your programm must start with: %s <FILE_IN> <FILE_OUT>\n", argv[0]);
        exit(1);
    }
    FILE* input = fopen(argv[1], "r");
    if (input == NULL) {
        printf("Error oppening file!!!\n");
        exit(2);
    }
    FILE* output = fopen(argv[2], "w");
    if (output == NULL) {
        printf("Error oppening file!!!\n");
        fclose(input);
        exit(2);
    }
    char symbol = fgetc(input);
    while (symbol != EOF) {
        fprintf(output, "%c", symbol);
        symbol = fgetc(input);
    }
    fclose(input);
    fclose(output);
    return 0;
}