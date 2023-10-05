#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Your programm must start with: %s <file_path>\n", argv[0]);
        exit(1);
    }

    FILE* file = fopen(argv[1], "wb");
    if (!file) {
        printf("Error, can`t open file!!!\n");
        exit(1);
    }

    unsigned char bytes[] = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    fwrite(bytes, sizeof(unsigned char), sizeof(bytes), file);

    fclose(file);

    file = fopen(argv[1], "rb");
    if (!file) {
        printf("Error, can`t open file!!!\n");
        exit(1);
    }

    unsigned char buffer;
    //size_t bytes_read;
    while (fread(&buffer, sizeof(unsigned char), 1, file)) {
        printf("Read byte: %u\n", buffer);
        printf("File position: %ld\n", ftell(file));
        printf("EOF indicator: %d\n", feof(file));
        printf("Error indicator: %d\n\n", ferror(file));
    }
    fclose(file);

    file = fopen(argv[1], "rb");
    if (!file) {
        printf("Error, can`t open file!!!\n");
        exit(1);
    }
    fseek(file, 3, SEEK_SET);
    unsigned char read_buffer[4];
    fread(read_buffer, sizeof(unsigned char), sizeof(read_buffer), file);
    printf("\nBuffer contents after fseek and fread: ");
    for (size_t i = 0; i < sizeof(read_buffer); ++i) {
        printf("%u ", read_buffer[i]);
    }
    printf("\n");

    fclose(file);

    return 0;
}
