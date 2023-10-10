#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "headers/lab.h"

status_code read_lexems_sanctions(const char* input, char** user_login, int* num_suntions) {
    (*user_login) = (char*)malloc(STR_SIZE * sizeof(char));
    if (*user_login == NULL) {
        return code_error_malloc;
    }

    char cmd[STR_SIZE];  // Добавим массив для хранения cmd
    if (!sscanf(input, "%s %s %d", cmd, *user_login, num_suntions)) {
        free(*user_login);
        return code_invalid_parametr;
    }

    // Теперь скопируем cmd в user_login
    strcpy(*user_login, cmd);

    return code_succes;
}


status_code read_lexems_date(const char* input, char** flag, char** time) {
    char* cmd;
    (*time) = (char*)malloc(STR_SIZE * sizeof(char));
    if (*time == NULL) {
        return code_error_malloc;
    }
    (*flag) = (char*)malloc(3 * sizeof(char));
    if (*flag == NULL) {
        return code_error_malloc;
    }
    if (!sscanf(input, "%s %s %s", cmd, *time, *flag)) {
        free(*flag);
        free(*time);
        return code_invalid_parametr;
    }
    printf("%s-----%s------%s", input, *flag, *time);
    return code_succes;

}

int main(int argc, char *argv[]) {
    Node* storage = NULL;
    bool authorized = false;
    int pincode, number_sanctions;
    char login[6];
    char choose[STR_SIZE];
    User* _user;
    char cmd[STR_SIZE];
    char input_arg2[STR_SIZE];
    char input_arg3[STR_SIZE];
    int counter_procedures = 0;
    while (true) {
        if (!authorized) {
           print_menu_non_authorized(); 
           printf("login: ");
           scanf("%s", login);
           printf("Pincode: ");
           scanf("%d", &pincode);
           status_code creation = create_user(login, pincode, &_user);
           switch (creation) {
                code_succes:
                    break;
                code_error_malloc:
                    printf("Error malloc detected!\n");
                    break;
                code_invalid_parametr:
                    printf("Invalid parameter detected!\n");
                    break;
           }
           if (creation == code_error_malloc || creation == code_invalid_parametr) {
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
            print_menu_authorized();
            if (get_sanctions(_user) != 0) {
                printf("You have only %d procedures\n", _user->sanctions);
            }
            scanf("%s", choose);
            if (!strcmp(choose, "Time")) {
                get_time();
                if (get_sanctions(_user) != 0) {
                    counter_procedures++;
                }
                continue;
            } else if (!strcmp(choose, "Date")) {
                get_date();
                if (get_sanctions(_user) != 0) {
                    counter_procedures++;
                }
                continue;
            } else if (!strcmp(choose, "Logout")) {
                if (get_sanctions(_user) != 0) {
                    counter_procedures = 0;
                }
                printf("You have successfully logged out of your account");
                authorized = false;
                continue;
            } else {
                sscanf(choose, "%s %s %s ", cmd, input_arg2, input_arg3);
                if (!strcmp(cmd, "Howmuch")) {
                    status_code status_time = get_elapsed_time(input_arg2, input_arg3);
                    if (status_time == code_invalid_parametr) {
                        printf("Invalid parameter detected!\n");
                    }
                    continue;
                } else if (!strcmp(cmd, "Sanctions")) {
                    int approve_value;
                    printf("Enter 12345 to accept sanctions: ");
                    scanf("%d", &approve_value);
                    getchar();
                    printf("%s----%s\n", input_arg2, input_arg3);
                    if (approve_value == 12345) {
                        switch (make_sanctions(storage, input_arg2, input_arg3)) {
                            case code_invalid_parametr:
                                printf("Invalid parameter detected!!!\n");
                                break;
                            case code_succes:
                                printf("Sanctions applied to the %s\n", input_arg2);
                                break;
                            default:
                                break;
                        }
                    } else {
                        printf("Your command is invalid, try later\n");
                        continue;
                    }

                }
        }
        }

    }
    return 0;
}
