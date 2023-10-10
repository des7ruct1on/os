#include "../headers/lab.h"

int get_sanctions(User* id) {
    return id->sanctions;
    
}

status_code_register register_user(Node** head, User* person) {
    char* log = person->login;
    int pin = person->pincode;
    status_code_register status_exist = find_user(*head, log, pin);
    switch (status_exist) {
        case code_register_authorized:
            return status_exist;
        case code_register_non_authorized:
            break;
        case code_register_invalid_parametr:
        default:
            break;
    }
    Node* new_node = (Node*)malloc(sizeof(Node));
    if (new_node == NULL) {
        return code_error_malloc;
    }
    new_node->data = *person;
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

status_code_register find_user(Node* list, const char* log, int pin) {
    if (strlen(log) > 6) {
        return code_register_invalid_parametr;
    }
    Node* current = list;
    while (current != NULL) {
        User check = current->data;
        if (!strcmp(check.login, log) && check.pincode != pin) {
            return code_register_invalid_parametr;
        } else if (!strcmp(check.login, log) && check.pincode == pin) {
            return code_register_authorized;
        }
        current = current->next;
    }
    return code_register_non_authorized;
}

status_code make_sanctions(Node* list, const char* log, char* number) {
    if (strlen(log) > 6) {
        printf("%s\n", log);
        return code_invalid_parametr;
    }
    int number_sanctions;
    sscanf(number, "%d", &number_sanctions);
    Node* current = list;
    while (current != NULL) {
        User *user = &(current->data);  // Получаем указатель на данные в списке
        if (!strcmp(user->login, log)) {
            user->sanctions = number_sanctions;  // Изменяем данные в списке
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
    printf("|==========================================================|\n");
}

void get_time() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current time: %02d:%02d:%02d\n", time_info->tm_hour, time_info->tm_min, time_info->tm_sec);
}

// Функция для выполнения команды Date
void get_date() {
    time_t raw_time;
    struct tm *time_info;

    time(&raw_time);
    time_info = localtime(&raw_time);

    printf("Current date: %02d:%02d:%04d\n", time_info->tm_mday, time_info->tm_mon + 1, time_info->tm_year + 1900);
}

// Функция для выполнения команды Howmuch
status_code get_elapsed_time(char *time_start, char *flag) {
    if (!strcmp(flag, "-s") || !strcmp(flag, "-m") || !strcmp(flag, "-h") || !strcmp(flag, "-y")) {
        return code_invalid_parametr;
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

    if (strcmp(flag, "-s") == 0) {
        printf("Elapsed time in seconds: %.0f\n", elapsed_seconds);
    } else if (strcmp(flag, "-m") == 0) {
        printf("Elapsed time in minutes: %.2f\n", elapsed_seconds / 60.0);
    } else if (strcmp(flag, "-h") == 0) {
        printf("Elapsed time in hours: %.2f\n", elapsed_seconds / 3600.0);
    } else if (strcmp(flag, "-y") == 0) {
        printf("Elapsed time in years: %.4f\n", elapsed_seconds / (365.25 * 24 * 3600));
    }
    return code_succes;
}
