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

typedef enum status_cmd {
    cmd_time,
    cmd_date,
    cmd_logout,
    cmd_howmuch,
    cmd_sanctions,
    cmd_error_malloc,
    cmd_invalid_parameter
} status_cmd;

typedef enum status_free {
    free_success,
    free_error
} status_free;

status_free free_all(int count, ...) {
    if (count < 1) {
        return free_error;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
    }

    va_end(ptr);
    return free_success;
}

status_cmd command(char** arg_one, char** arg_two) {
    char* tmp_cmd = (char*)malloc(STR_SIZE * sizeof(char));
    if (tmp_cmd == NULL) {
        return code_error_malloc;
    }
    int index = 0;
    char symbol_choose = getchar();
    while(!isspace(symbol_choose)) {
        tmp_cmd[index++] = symbol_choose;
        symbol_choose = getchar();
    }
    tmp_cmd[index] = '\0';
    index = 0;
    if (!strcmp(tmp_cmd, "Time")) {
        free(tmp_cmd);
        return cmd_time;
    } else if (!strcmp(tmp_cmd, "Date")) {
        free(tmp_cmd);
        return cmd_date;
    } else if (!strcmp(tmp_cmd, "Howmuch")) {
        free(tmp_cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return code_error_malloc;
        }
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            free(*arg_one);
            return code_error_malloc;
        }
        while(isspace(symbol_choose)) {
            symbol_choose = getchar();
            if (symbol_choose == '\0') {
                free(*arg_one);
                free(*arg_two);
                return cmd_invalid_parameter;
            }
        }
        while(!isspace(symbol_choose)) {
            (*arg_one)[index++] = symbol_choose;
            symbol_choose = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        while(isspace(symbol_choose)) {
            symbol_choose = getchar();
            if (symbol_choose == '\0') {
                free(*arg_one);
                free(*arg_two);
                return cmd_invalid_parameter;
            }
        }
        while(!isspace(symbol_choose)) {
            (*arg_two)[index++] = symbol_choose;
            symbol_choose = getchar();
        }
        (*arg_two)[index] = '\0';
        return cmd_howmuch;
    } else if (!strcmp(tmp_cmd, "Sanctions")) {
        free(tmp_cmd);
        (*arg_one) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_one == NULL) {
            return code_error_malloc;
        }
        (*arg_two) = (char*)malloc(STR_SIZE * sizeof(char));
        if (*arg_two == NULL) {
            free(*arg_one);
            return code_error_malloc;
        }
        while(isspace(symbol_choose)) {
            symbol_choose = getchar();
            if (symbol_choose == '\0') {
                free(*arg_one);
                free(*arg_two);
                return cmd_invalid_parameter;
            }
        }
        while(!isspace(symbol_choose)) {
            (*arg_one)[index++] = symbol_choose;
            symbol_choose = getchar();
        }
        (*arg_one)[index] = '\0';
        index = 0;
        while(isspace(symbol_choose)) {
            symbol_choose = getchar();
            if (symbol_choose == '\0') {
                free(*arg_one);
                free(*arg_two);
                return cmd_invalid_parameter;
            }
        }
        while(!isspace(symbol_choose)) {
            (*arg_two)[index++] = symbol_choose;
            symbol_choose = getchar();
        }
        (*arg_two)[index] = '\0';
        return cmd_sanctions;
    } else if (!strcmp(tmp_cmd, "Logout")) {
        free(tmp_cmd);
        return cmd_logout;
    } else {
        free(tmp_cmd);
        return cmd_invalid_parameter;
    }
}

int main(int argc, char *argv[]) {
    Node* storage = NULL;
    bool authorized = false;
    int pincode, number_sanctions;
    char login[32];
    char cmd[STR_SIZE];
    char *input_arg2 = NULL;
    char *input_arg3 = NULL;
    int counter_procedures = 0;
    int approve_value;
    while (true) {
        User* _user;
        if (!authorized) {
           print_menu_non_authorized(); 
           printf("login: ");
           scanf("%s", login);
           getchar();
           printf("Pincode: ");
           scanf("%d", &pincode); 
           getchar();      
           status_code creation = create_user(login, pincode, &_user);
           switch (creation) {
                code_succes:
                    break;
                code_error_malloc:
                    printf("Error malloc detected!\n");
                    exit(1);
                    break;
                code_invalid_parametr:
                    printf("Invalid parameter detected, try again!\n");
                    break;
           }
           if (creation == code_invalid_parametr) {
                continue;
           }
           status_code_register status_exist = register_user(&storage, _user);
           switch (status_exist) {
                case code_register_authorized:
                    printf("You successfully authorized, user: %s\n", login);
                    authorized = true;
                    break;
                case code_register_non_authorized:
                    printf("You successfully registered, user: %s\n", login);
                    authorized = true;
                    break;
                case code_register_invalid_parametr:
                    printf("Invalid parameter detected!!!\n");
                    break;
                case code_register_error_malloc:
                    printf("Error malloc detected!\n");
                    break;
           }
            if (status_exist == code_register_invalid_parametr || status_exist == code_register_error_malloc) {
                continue;
            }
           
        } else {
            status_free free_st;
            print_menu_authorized();
            if (_user->sanctions - counter_procedures == 0 && _user->sanctions != 0) {
                _user->sanctions = 0;
                printf("You can`t do anything in this session. Logging out...\n");
                authorized = false;
                free_st = free_all(2, input_arg2, input_arg3);
                if (free_st == free_error) {
                    destroy_storage(&storage);
                    printf("Error detected, while freeing!\n");
                    exit(2);
                }
                continue;
            }
            if (get_sanctions(_user) != 0) {
                printf("You have only %d procedures\n", _user->sanctions - counter_procedures);
            }
            status_cmd cmd = command(&input_arg2, &input_arg3);
            switch (cmd) {
                case cmd_time:
                    if (get_sanctions(_user) != 0) {
                        counter_procedures++;
                    }
                    get_date();
                    break;
                case cmd_date: 
                    if (get_sanctions(_user) != 0) {
                        counter_procedures++;
                    }
                    get_date();
                    break;
                case cmd_howmuch:
                    if (get_sanctions(_user) != 0) {
                        counter_procedures++;
                    }
                    status_code elaps_status = get_elapsed_time(input_arg2, input_arg3);
                    if (elaps_status == code_invalid_parametr) {
                        printf("Invalid parameter detected, try again!\n");
                    }
                    break;
                case cmd_sanctions:
                    printf("Enter 12345 to confirm: ");
                    scanf("%d", &approve_value);
                    getchar();  
                    if (approve_value == 12345) {
                        if (get_sanctions(_user) != 0) {
                            counter_procedures++;
                        }
                        status_code sanction_status = make_sanctions(storage, input_arg2, input_arg3);
                        if (sanction_status == code_invalid_parametr) {
                            printf("Invalid parameter detected, try again!\n");
                        }
                    } else {
                        printf("Cancelled!\n");
                    }
                    break;
                case cmd_logout:
                    if (get_sanctions(_user) != 0) {
                        counter_procedures = 0;
                    }
                    printf("You have successfully logged out of your account");
                    authorized = false;
                    break;
                case cmd_error_malloc:
                    printf("Error malloc detected!!!\n");
                    destroy_storage(&storage);
                    exit(2);
                case cmd_invalid_parameter:
                    printf("Invalid parameter detected, try again!\n");
                    break;
            }
            free_st = free_all(2, input_arg2, input_arg3);
            if (free_st == free_error) {
                destroy_storage(&storage);
                printf("Error detected, while freeing!\n");
                exit(2);
            }
        }

    }
    destroy_storage(&storage);
    return 0;
}
