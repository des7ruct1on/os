#ifndef lab_h
#define lab_h

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include <ctype.h>
#include <stdarg.h>
#define MAX_PINCODE 100000
#define STR_SIZE 256

typedef enum status_code {
    code_succes,
    code_error_malloc,
    code_invalid_parametr
} status_code;

typedef enum status_code_register {
    code_register_error_malloc,
    code_register_invalid_parametr, 
    code_register_authorized,
    code_register_non_authorized
} status_code_register;

typedef enum status_cmd {
    cmd_time,
    cmd_date,
    cmd_logout,
    cmd_howmuch,
    cmd_sanctions,
    cmd_error_malloc,
    cmd_invalid_parameter,
    cmd_exit
} status_cmd;

typedef enum status_free {
    free_success,
    free_error
} status_free;

typedef struct User {
    char login[6];
    int pincode;
    int sanctions;
} User;

typedef struct Node { 
    User* data;
    struct Node* next;
    struct Node* prev;
} Node;

status_free free_all(int count, ...);
status_cmd command(char** arg_one, char** arg_two);
status_code_register register_user(Node** head, User** person);
status_code_register find_user(Node* list, const char* log, int pin, User** user);
status_code destroy_storage(Node** head);
status_code create_user(const char* log, int pin, User** person);
status_code make_sanctions(Node* list, const char* log, char* number, User* user);
status_code get_elapsed_time(char* time_start, char *flag);
void print_menu_non_authorized();
void print_menu_authorized();
void get_time();
void get_date();
int get_sanctions(User* id);
#endif