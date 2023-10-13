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

int main(int argc, char *argv[]) {
    Node* storage = NULL;
    bool authorized = false;
    int pincode, number_sanctions;
    char login[32];
    char cmd[STR_SIZE];
    int counter_procedures = 0;
    int approve_value;
    User* _user;
    while (true) {
        char *input_arg2 = NULL;
        char *input_arg3 = NULL;
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
           status_code_register status_exist = register_user(&storage, &_user);
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
            status_code elaps_status;
            status_free free_st;
            status_code sanction_status;
            print_menu_authorized();
            if (_user->sanctions - counter_procedures == 0 && _user->sanctions != 0) {
                _user->sanctions = 0;
                counter_procedures = 0;
                printf("You can`t do anything in this session. Logging out...\n");
                authorized = false;
                free_st = free_all(2, input_arg2, input_arg3);
                if (free_st == free_error) {
                    printf("Error while freeing detected!\n");
                    destroy_storage(&storage);
                    exit(3);
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
                    get_time();
                    break;
                case cmd_date: 
                    if (get_sanctions(_user) != 0) {
                        counter_procedures++;
                    }
                    get_date();
                    break;
                case cmd_howmuch:
                    elaps_status = get_elapsed_time(input_arg2, input_arg3);
                    if (elaps_status == code_invalid_parametr) {
                        printf("Invalid parameter detected, try again!\n");
                    }
                    if (get_sanctions(_user) != 0 && elaps_status != code_invalid_parametr) {
                        counter_procedures++;
                    }
                    break;
                case cmd_sanctions:
                    printf("Enter 12345 to confirm: ");
                    scanf("%d", &approve_value);
                    getchar();  
                    if (approve_value == 12345) {
                        sanction_status = make_sanctions(storage, input_arg2, input_arg3, _user);
                        if (sanction_status == code_invalid_parametr) {
                            printf("Invalid parameter detected, try again!\n");
                        }
                        if (get_sanctions(_user) != 0 && sanction_status != code_invalid_parametr) {
                            counter_procedures++;
                        }
                    } else {
                        printf("Cancelled!\n");
                    }
                    break;
                case cmd_logout:
                    if (get_sanctions(_user) != 0) {
                        counter_procedures = 0;
                        _user->sanctions = 0;
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
                case cmd_exit:
                    break;
            }
            free_st = free_all(2, input_arg2, input_arg3);
            if (free_st == free_error) {
                printf("Error while freeing detected!\n");
                destroy_storage(&storage);
                exit(2);
            }
            if (cmd == cmd_exit) {
                break;
            }
        }  
    }


    destroy_storage(&storage);
    return 0;
}
