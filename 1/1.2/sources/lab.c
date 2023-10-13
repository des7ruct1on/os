#include "../headers/lab.h"

int get_sanctions(User* id) {
    return id->sanctions;
    
}

status_code_register register_user(Node** head, User** person) {
    char* log = (*person)->login;
    int pin = (*person)->pincode;
    status_code_register status_exist = find_user(*head, log, pin, person);
    switch (status_exist) {
        case code_register_authorized:
            return status_exist;
        case code_register_non_authorized:
            break;
        case code_register_invalid_parametr:
        default:
            break;
    }
    //printf("123324324\n");
    (*person)->sanctions = 0;
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return code_error_malloc;
    }
    new_node->data = (*person);
    new_node->next = NULL;

    if (*head == NULL) {
        new_node->prev = NULL;
        *head = new_node;
        return status_exist;
    }

    Node* current = *head;
    while (current->next != NULL) {
        current = current->next;
    }

    current->next = new_node;
    new_node->prev = current;
    return status_exist;
}

status_code destroy_storage(Node** head) {
    Node* current = *head;
    Node* next;

    while (current != NULL) {
        next = current->next;
        free(current->data);
        free(current);
        current = next;
    }

    *head = NULL;
    return code_succes;
}

status_code create_user(const char* log, int pin, User** person) {
    if (pin > MAX_PINCODE || strlen(log) > 6) {
        return code_invalid_parametr;
    }
    *person = malloc(sizeof(User));
    if (*person == NULL) {
        return code_error_malloc;
    }
    strcpy((*person)->login, log);
    (*person)->pincode = pin;
    (*person)->sanctions = 0;
    return code_succes;
}

status_code_register find_user(Node* list, const char* log, int pin, User** user) {
    if (strlen(log) > 6) {
        return code_register_invalid_parametr;
    }
    Node* current = list;
    while (current != NULL) {
        User* check = current->data;
        if (!strcmp(check->login, log) && check->pincode != pin) {
            return code_register_invalid_parametr;
        } else if (!strcmp(check->login, log) && check->pincode == pin) {
            free(*user);
            *user = check;
            return code_register_authorized;
        }
        current = current->next;
    }
    return code_register_non_authorized;
}

status_code make_sanctions(Node* list, const char* log, char* number, User* user) {
    if (strlen(log) > 6) {
        return code_invalid_parametr;
    }
    printf("%s %s %d\n", user->login, log, user->sanctions);
    if (!strcmp(user->login, log) && user->sanctions != 0) {
        return code_invalid_parametr;
    }
    int number_sanctions;
    sscanf(number, "%d", &number_sanctions);
    if (number_sanctions < 1) {
        return code_invalid_parametr;
    }
    Node* current = list;
    while (current != NULL) {
        User* user = current->data;  
        if (!strcmp(user->login, log)) {
            user->sanctions = number_sanctions;  
            return code_succes;
        }
        current = current->next;
    }
    return code_invalid_parametr;
}


void print_menu_non_authorized() {
    printf("\n|===============|Welcome to the programm!!!|===============|\n");
    printf("| Enter your login and password to continue                |\n");
    printf("|==========================================================|\n");
}

void print_menu_authorized() {
    printf("|==========================================================|\n");
    printf("| Enter \"Time\" to know whats time is now                 |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Date\" to know whats date is today               |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Howmuch <time> flag\" to know how much time left |\n");
    printf("| after specified date, flag = '-s', '-m', '-h', '-y'      |\n");
    printf("| so you can get answer in sec / min / hours / years       |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Logout\" to log out from account                 |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Sanctions username <number>\" to create sanctions|\n");
    printf("| on specified user with specified quantity                |\n");
    printf("|----------------------------------------------------------|\n");
    printf("| Enter \"Exit\" to shut down programm                     |\n");
    printf("|==========================================================|\n");
}

void get_time() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current time: %02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

void get_date() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current date: %02d:%02d:%04d\n", time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
}

status_code get_elapsed_time(char *time_start, char *flag) {
    if (flag[0] != '-') {
        return code_invalid_parametr;
    }
    switch (flag[1]) {
        case 's':
        case 'm':
        case 'h':
        case 'y':
            break;
        default:
            code_invalid_parametr;
    }
    struct tm tm_start;
    int day, month, year;
    if (sscanf(time_start, "%d:%d:%d", &day, &month, &year) != 3) {
        return code_invalid_parametr;
    }

    tm_start.tm_year = year - 1900;
    tm_start.tm_mon = month - 1;
    tm_start.tm_mday = day;
    tm_start.tm_hour = 0;
    tm_start.tm_min = 0;
    tm_start.tm_sec = 0;

    time_t start_time = mktime(&tm_start);
    time_t current_time = time(NULL);

    double elapsed_seconds = difftime(current_time, start_time);
    if (elapsed_seconds < 0) {
        return code_invalid_parametr;
    }
    if (flag[1] == 's') {
        printf("Elapsed time in seconds: %.0f\n", elapsed_seconds);
    } else if (flag[1] == 'm') {
        printf("Elapsed time in minutes: %.2f\n", elapsed_seconds / 60.0);
    } else if (flag[1] == 'h') {
        printf("Elapsed time in hours: %.2f\n", elapsed_seconds / 3600.0);
    } else if (flag[1] == 'y') {
        printf("Elapsed time in years: %.4f\n", elapsed_seconds / (365.25 * 24 * 3600));
    }
    return code_succes;
}

status_free free_all(int count, ...) {
    if (count < 1) {
        return free_error;
    }

    va_list ptr;
    va_start(ptr, count);

    for (int i = 0; i < count; i++) {
        void* ptr_to_free = va_arg(ptr, void*);
        free(ptr_to_free);
        ptr_to_free = NULL;
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
    } else if (!strcmp(tmp_cmd, "Exit")) {
        free(tmp_cmd);
        return cmd_exit;
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
