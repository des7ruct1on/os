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
    if (feof(file)) {
        *index_line = -1;
        return code_success;
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

status_code get_names(char*** names, FILE* in, int* capacity_names) {
    if (feof(in)) {
        return code_success;
    }
    *capacity_names = 32;
    int capacity_row = 32;
    int index_name = 0;
    int index_row = 0;
    *names = (char**)malloc((*capacity_names) * sizeof(char*));
    if (*names == NULL) {
        return code_alloc_error;
    }
    char c;
    c = fgetc(in);
    char* tmp = (char*)malloc(sizeof(char) * capacity_row);
    if (!tmp) {
        free(*names);
        return code_alloc_error;
    }
    bool started = false;
    while (true) {
        if (isspace(c) || c == '\n' || c == EOF) {
            if (started) {
                started = false;
                tmp[index_row] = '\0';
                (*names)[index_name] = strdup(tmp);
                index_name++;
                if (index_name == *capacity_names - 1) {
                    (*capacity_names) *= 2;
                    status_realloc st_realloc = my_realloc(names, *capacity_names);
                    if (st_realloc == status_realloc_fail) {
                        free(tmp);
                        free(*names);
                        return code_alloc_error;
                    }
                }
                free(tmp);
                tmp = (char*)malloc(sizeof(char) * capacity_row);
                if (!tmp) {
                    free(*names);
                    return code_alloc_error;
                }
                index_row = 0;
                if (c == EOF) {
                    break;
                }
            }
            c = fgetc(in);
        } else if (!isspace(c)) {
            started = true;
            tmp[index_row] = c;
            index_row++;
            if (index_row == capacity_row - 1) {
                capacity_row *= 2;
                status_realloc st_realloc = my_realloc(&tmp, capacity_row);
                if (st_realloc == status_realloc_fail) {
                    free(tmp);
                    free(*names);
                    return code_alloc_error;
                }
            }
            c = fgetc(in);
        }
    }
    free(tmp);
    *capacity_names = index_name;
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
    char** names = NULL;
    status_code st_read_names = get_names(&names, f_names, &capacity);
    if (st_read_names == code_alloc_error) {
        free(names);
        printf("Error alloc detected!!!\n");
        fclose(f_names);
        return -3;
    }
    pid_t pid;
    int count = 0;
    for (int i = 0; i < capacity; i++) {
        char* current_file = names[i];
        pid = fork();
        if (pid == 0) {
            size_t index_row, index_line;
            status_code st_find = find_substring(current_file, substring, &index_row, &index_line);
            if (st_find == code_error_opening_file) {
                printf("Can`t open file: %s!\n", current_file);
                fclose(f_names);
                free(names);
                return -3;
            } else if (st_find == code_invalid_parameter) {
                printf("Invalid parameter detected!!!\n");
                fclose(f_names);
                free(names);
                return -4;
            }
            if (index_line != -1) {
                printf("%s found at %ld pos, at %ld line in %s\n", substring, index_row, index_line, current_file);
            } else {
                printf("%s has not %s\n", current_file, substring);
            }
            exit(0);
        } else if (pid == -1) {
            exit(-1);
        } else {
            if (!current_file) {
                printf("Error malloc detected!!!\n");
                fclose(f_names);
                return -7;
            }
        }
    }
    free(names);
    fclose(f_names);

    return 0;
}
