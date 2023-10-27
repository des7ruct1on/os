#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <ctype.h>
#include <stdbool.h>

typedef enum status_code {
    code_invalid_parameter,
    code_error_opening_file,
    code_alloc_error,
    code_success
} status_code;

typedef enum status_realloc {
    status_realloc_ok,
    status_realloc_fail
} status_realloc;

status_realloc my_realloc(void** var, int size) {
    void* new_ptr = realloc(*var, size);
    if (new_ptr != NULL) {
        *var = new_ptr;
        return status_realloc_ok;
    } else {
        return status_realloc_fail;
    }
}

status_code find_substring(const char *file_path, const char *substring, size_t* index_row, size_t* index_line) {
    FILE* file = fopen(file_path, "r");
    if (file == NULL) {
        return code_error_opening_file;
    }

    char *buffer = NULL;
    *index_line = 1;
    int read;

    while ((read = getline(&buffer, &(size_t){0}, file)) != -1) {
        char *found = strstr(buffer, substring);
        if (found != NULL) {
            *index_row = found - buffer;
            free(buffer);
            fclose(file);
            return code_success;
        }

        (*index_line)++;
    }
    (*index_line) = -1;
    free(buffer);
    fclose(file);
    return code_success;
}

int main(int argc, char *argv[]) {
    if (argc < 3) {
        printf("Your program must start with: %s <file_names> <substring>\n", argv[0]);
        return -1;
    }

    const char* file_names = argv[1];
    const char* substring = argv[2];
    FILE* f_names = fopen(file_names, "r");
    if (!f_names) {
        printf("Can't open file!\n");
        return -2;
    }
    int capacity = 32;
    char* tmp_file_name = malloc(sizeof(char) * capacity);
    if (!tmp_file_name) {
        printf("Error malloc detected!!!\n");
        fclose(f_names);
        return -7;
    }
    int index = 0;
    char c;
    pid_t pid;
    bool start = true;
    status_realloc st_realloc;
    while (!feof(f_names)) {
        c = fgetc(f_names);
        if (c == EOF || feof(f_names)) {
            break;
        }
        while (isspace(c)) {
            c = fgetc(f_names);
        }
        while (!isspace(c)) {
            tmp_file_name[index] = c;
            index++;
            if (index == capacity - 1) {
                capacity *= 2;
                st_realloc = my_realloc(&tmp_file_name, capacity);
                if (st_realloc == status_realloc_fail) {
                    printf("Error realloc detected!!!\n");
                    free(tmp_file_name);
                    fclose(f_names);
                    return -5;

                }
            }
            c = fgetc(f_names);
            if (c == EOF) {
                break;
            }
        }
        tmp_file_name[index] = '\0';
        index = 0;
        size_t index_row, index_line;
        pid = fork();
        if (pid == 0) {
            status_code st_find = find_substring(tmp_file_name, substring, &index_row, &index_line);
            if (st_find == code_error_opening_file) {
                printf("Can`t open file: %s!\n", tmp_file_name);
                free(tmp_file_name);
                fclose(f_names);
                return -3;
            } else if (st_find == code_invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                free(tmp_file_name);
                fclose(f_names);
                return -4;
            }
            if (index_line != -1) {
                printf("%s found at %ld pos, at %ld line in %s\n", substring, index_row, index_line, tmp_file_name);
            } else {
                printf("%s has not %s\n", tmp_file_name, substring);
            }
            exit(0);
        } else if (pid == -1) {
            free(tmp_file_name);
            tmp_file_name = NULL;
            exit(-1);
        } else {
            if (feof(f_names)) {
                free(tmp_file_name);
                break;
            }
            free(tmp_file_name);
            tmp_file_name = NULL; 
            capacity = 32;
            index = 0;
            tmp_file_name = malloc(sizeof(char) * capacity);
            if (!tmp_file_name) {
                printf("Error malloc detected!!!\n");
                fclose(f_names);
                return -7;
            }
        }
    }

    fclose(f_names);

    return 0;
}
