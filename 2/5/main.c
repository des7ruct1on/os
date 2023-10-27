#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX_PEOPLE 5
typedef enum Gender {
    MALE,
    FEMALE, 
    UNKNOWN
} Gender;

pthread_cond_t female_cond, male_cond;
int count_bathroom = 0;
pthread_mutex_t mutex;
Gender sign = UNKNOWN;

void male_enter() {
    pthread_mutex_lock(&mutex);
    while (count_bathroom == MAX_PEOPLE || sign == FEMALE) {
        pthread_cond_wait(&male_cond, &mutex);
    }
    sign = MALE;
    count_bathroom++;
    printf("-> Man entered the bath\n");
    printf("\nBath is for MALE rn\n");
    printf("    People in bathroom = %d\n", count_bathroom);
    pthread_mutex_unlock(&mutex);
}

void male_leave() {
    pthread_mutex_lock(&mutex);
    count_bathroom--;
    printf("<- One man is out of bathroom\n");
    if (count_bathroom == 0) {
        sign = UNKNOWN;
        printf("\nBath is free rn\n");
        pthread_cond_broadcast(&male_cond);
    }
    printf("    People in bathroom = %d\n", count_bathroom);
    pthread_mutex_unlock(&mutex);
}

void* get_man_thread(void* arg) {
    const int man_count = 1;
    for (int i = 0; i < man_count; i++) {
        male_enter();
        male_leave();
    }
    return NULL;
}

void female_leave() {
    pthread_mutex_lock(&mutex);
    count_bathroom--;
    printf("<- One woman is out of bathroom\n");
    if (count_bathroom == 0) {
        sign = UNKNOWN;
        printf("\nBath is free rn\n");
        pthread_cond_broadcast(&female_cond);
    }
    printf("    People in bathroom = %d\n", count_bathroom);
    pthread_mutex_unlock(&mutex);
}

void female_enter() {
    pthread_mutex_lock(&mutex);
    while (count_bathroom == MAX_PEOPLE || sign == MALE) {
        pthread_cond_wait(&male_cond, &mutex);
    }
    sign = FEMALE;
    count_bathroom++;
    printf("-> Woman entered the bath\n");
    printf("\nBath is for FEMALE rn\n");
    printf("    People in bathroom = %d\n", count_bathroom);
    pthread_mutex_unlock(&mutex);
}


void* get_woman_thread(void* arg) {
    const int man_count = 1;
    for (int i = 0; i < man_count; i++) {
        female_enter();
        female_leave();
    }
    return NULL;
}

void bath_solution(int count_man, int count_woman) {
    pthread_t male[count_man];
    pthread_t female[count_woman];
    for (int i = 0; i < count_man; i++) {
        pthread_create(&male[i], NULL, get_man_thread, NULL);
    }
    for (int i = 0; i < count_man; i++) {
        pthread_create(&female[i], NULL, get_woman_thread, NULL);
    }
    for (int i = 0; i < count_man; i++) {
        pthread_join(male[i], NULL);
    }
    for (int i = 0; i < count_man; i++) {
        pthread_join(female[i], NULL);
    }
}


int main(int argc, char* argv[]) {
    unsigned int count_man, count_woman;
    printf("Enter number of men: ");
    scanf("%d", &count_man);
    printf("Enter number of women: ");
    scanf("%d", &count_woman);
    if (count_man > MAX_PEOPLE || count_woman > MAX_PEOPLE) {
        printf("Invalid parameter detected!!!\n");
        exit(-1);
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_cond_init(&female_cond, NULL);
    pthread_cond_init(&male_cond, NULL);

    bath_solution(count_man, count_woman);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&female_cond);
    pthread_cond_destroy(&male_cond);

    sleep(1); 
    return 0;
}