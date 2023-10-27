#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

enum ERRORS
{
    success = 0,
    incorrect_input = -1,
    file_open_error = -2,
    no_memory = -3,
    fail = -4,
    create_process_fail = -5
};

int search_string(const char * file_name, const char * substring, int *count);
void print_error(int error);

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        print_error(incorrect_input);
        return incorrect_input;
    }
    FILE * main_file = fopen(argv[1], "r");
    if (!main_file)
    {
        print_error(file_open_error);
        return file_open_error;
    }

    pid_t pid;
    const char * substring = "zxc";
    char symbol;
    int length = 1, count = 0;
    char * file_name = (char*)malloc(length * sizeof(char));
    if (file_name == NULL) return no_memory;
    char * temp;
    int count_success_files = 0;
    int result, status;
    while ((symbol = fgetc(main_file)) != EOF)
    {
        if (symbol == ' ' || symbol == '\n' || symbol == ',')
        {
            file_name[count] = '\0';
            count = 0;
            pid = fork();

            if (pid == -1)
            {
                free(file_name);
                file_name = NULL;
                print_error(create_process_fail);
                return create_process_fail;
            }
            else if (pid == 0)
            {
                result = search_string(file_name, substring, &count_success_files);
                if (result == file_open_error)
                {
                    free(file_name);
                    file_name = NULL;
                    print_error(result);
                    return result;
                }
                if (result == success)
                {
                    printf("%s\n", file_name);
                }
                exit(count_success_files);
            }
            else
            {
                free(file_name);
                file_name = NULL;
                length = 1;
                count = 0;
                file_name = (char*)malloc(length * sizeof(char));
                if (file_name == NULL)
                {
                    print_error(no_memory);
                    return no_memory;
                }
            }
        }
        if (length <= count)
        {
            length *= 2;
            temp = (char*)realloc(file_name, length * sizeof(char));
            if (temp == NULL)
            {
                free(file_name);
                file_name = NULL;
                print_error(no_memory);
                return no_memory;
            }
            file_name = temp;
        }
        if (symbol != ' ' && symbol != '\n' && symbol != '\t')
        {
            file_name[count] = symbol;
            count++;
        }
    }
    free(file_name);
    file_name = NULL;
    fclose(main_file);

    pid_t child_pid;
    while ((child_pid = wait(&status)) > 0)
    {
        if (WIFEXITED(status)) count_success_files += WEXITSTATUS(status);
    }

    if (count_success_files == 0) printf("Cannot find substring in files.\n");

    return success;
}

int search_string(const char * file_name, const char * substring, int *count)
{
    FILE * file = fopen(file_name, "r");
    if (!file) return file_open_error;

    int length = strlen(substring), symbols_count = 0, all_symbols_count = 0;
    char temp_string[length + 1];
    temp_string[length] = 0;
    char symbol;
    while((symbol = fgetc(file)) != EOF)
    {
        if (symbols_count == length)
        {
            if (!strcmp(substring, temp_string))
            {
                fclose(file);
                *count += 1;
                return success;
            }
            symbols_count = 0;
        }
        temp_string[symbols_count] = symbol;
        symbols_count++;
        all_symbols_count++;
    }
    fclose(file);
    return fail;
}

void print_error(int error)
{
    switch(error)
    {
        case incorrect_input:
            printf("\nIncorrect input.\n");
            break;
        case file_open_error:
            printf("\nCannot open file.\n");
            break;
        case no_memory:
            printf("\nComputer doesnot have enough memory for this calculation.\n");
            break;
        case fail:
            printf("\nCannot run the function.\n");
            break;
        case create_process_fail:
            printf("\nCannot create process.\n");
            break;
    }
}