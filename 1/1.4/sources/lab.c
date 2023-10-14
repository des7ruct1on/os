#include "../headers/lab.h"

status_code xor8(const char* file_path, int* answer) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        return code_error_opening_file;
    }
    unsigned char byte;
    *answer = 0;
    while (fread(&byte, sizeof(char), 1, file)) {
        *answer ^= (int)byte;
    }
    fclose(file);
    return code_success;
}

status_code xor32(const char* file_path, int* answer) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        return code_error_opening_file;
    }
    unsigned char byte[4];
    *answer = 0;
    int count_read_bytes = 1;
    while (count_read_bytes) {
        count_read_bytes = fread(byte, sizeof(char), 4, file);
        int number_32_bit = 0;
        for (int i = 0; i < count_read_bytes; i++) {
            int offset = 0;
            offset = byte[i] << 8 * (3 - i);
            //printf("byte: %d, offset: %d\n", byte[i], offset);
            number_32_bit += offset;
        }
        //printf("%d\n", number_32_bit);
        *answer ^= number_32_bit;
    }
    fclose(file);
    return code_success;
}

status_code mask(const char* file_path, char* mask_hex, int* answer) {
    FILE *file = fopen(file_path, "rb");
    if (!file) {
        return code_error_opening_file;
    }
    unsigned int mask_number;
    sscanf(mask_hex, "%x", &mask_number);
    printf("%d\n", mask_number);
    unsigned int byte;
    *answer = 0;
    while(fread(&byte, sizeof(unsigned int), 1, file)) {
        printf("%d\n", byte);
        if (mask_number == byte) {
            (*answer)++;
        }
    }
    return code_success;

}

status_cmd command(char* argv[], int argc) {
    char* cmd = argv[2];
    printf("%s\n", cmd);
    if (!strcmp(cmd, "xor8")) {
        return cmd_xor8;
    } else if (!strcmp(cmd, "xor32")) {
        return cmd_xor32;
    } else if (!strcmp(cmd, "Mask")) {
        if (argc < 4) {
            return cmd_error_argc;
        }
        return cmd_mask;
    } else {
        return cmd_unknown;
    }
    
}
