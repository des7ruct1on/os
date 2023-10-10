#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "headers/lab.h"

int main(int argc, char *argv[]) {
    Node* storage = NULL;
    bool authorized = false;
    int pincode, number_sanctions;
    char login[6];
    char choose[STR_SIZE];
    char cmd[STR_SIZE];
    char time[STR_SIZE];
    User* _user;
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
                if (get_sanctions(_user) != 0) {
                    counter_procedures++;
                }
                sscanf(choose, "%s", cmd);
                getchar();
                if (!strcmp(cmd, "Howmuch")) {
                    char* flag;
                    sscanf(choose, "%s %s", time, flag);
                    //sscanf(choose, "%s", flag);
                    status_code status_time = get_elapsed_time(time, flag);
                    if (status_time == code_invalid_parametr) {
                        printf("Invalid parameter detected!\n");
                    }
                    continue;
                } else if (!strcmp(cmd, "Sanctions")) {
                    char approve_value[STR_SIZE];
                    sscanf(choose, "%s", login);
                    printf("%s-\n", login);
                    getchar();
                    sscanf(choose, "%d", &number_sanctions);
                    printf("%s-\n", login);
                    getchar();
                    printf("Enter 12345 to accept sanctions: ");
                    scanf("%s", approve_value);
                    getchar();
                    printf("%s----%d\n", login, number_sanctions);
                    if (!strcmp(approve_value, "12345")) {
                        switch (make_sanctions(storage, login, number_sanctions)) {
                            case code_invalid_parametr:
                                printf("Invalid parameter detected!!!\n");
                                break;
                            case code_succes:
                                printf("Sanctions applied to the %s\n", login);
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
