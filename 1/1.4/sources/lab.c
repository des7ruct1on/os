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
            number_32_bit += byte[i] << 8 * (3 - i);
        }
        printf("%d\n", number_32_bit);
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
    printf("!!!!!!!\n");
    sscanf(mask_hex, "%x", &mask_number);
    unsigned int byte;
    *answer = 0;
    while(fread(&byte, sizeof(unsigned int), 1, file)) {
        if (mask_number == byte) {
            (*answer)++;
        }
    }
    return code_success;

}

status_cmd command(char** arg_one, char* argv[], int argc) {
    int length = strlen(argv[2]);
    char* cmd = (char*)malloc((length + 1) * sizeof(char));
    for (int i = 0; i < length; i++) {
        cmd[i] = argv[2][i];
    }
    cmd[length] = '\0';
    printf("%s\n", cmd);
    if (cmd == NULL) {
        return cmd_error_malloc;
    }
    if (!strcmp(cmd, "xor8")) {
        free(cmd);
        return cmd_xor8;
    } else if (!strcmp(cmd, "xor32")) {
        free(cmd);
        return cmd_xor32;
    } else if (!strcmp(cmd, "Mask")) {
        free(cmd);
        if (argc < 4) {
            return cmd_error_argc;
        }
        int length_hex = strlen(argv[3]);
        char* mask_hex = (char*)malloc((length_hex + 1) * sizeof(char));
        if (mask_hex == NULL) {
            free(cmd);
            return cmd_error_malloc;
        }
        for (int i = 0; i < length_hex; i++) {
            mask_hex[i] = argv[3][i];
        }
        printf("%s\n", mask_hex);
        mask_hex[length_hex] = '\0';
        return cmd_mask;
    } else {
        free(cmd);
        return cmd_unknown;
    }
    
}
