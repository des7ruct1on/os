#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PHILOSOPHERS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t forks[NUM_PHILOSOPHERS];

void *philosopher(void *id) {
    int philosopher_id = *((int *)id);
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    while (true) {
        // Thinking
        printf("Philosopher %d is thinking.\n", philosopher_id);
        usleep(1000000); // Sleep for 1 second

        // Pick up left fork
        pthread_mutex_lock(&forks[left_fork]);
        printf("Philosopher %d picked up left fork (%d).\n", philosopher_id, left_fork);
        usleep(500000); // Sleep for 0.5 second

        // Pick up right fork
        pthread_mutex_lock(&forks[right_fork]);
        printf("Philosopher %d picked up right fork (%d).\n", philosopher_id, right_fork);

        // Eating
        printf("Philosopher %d is eating.\n", philosopher_id);
        usleep(1000000); // Sleep for 1 second

        // Put down right fork
        pthread_mutex_unlock(&forks[right_fork]);
        printf("Philosopher %d put down right fork (%d).\n", philosopher_id, right_fork);

        // Put down left fork
        pthread_mutex_unlock(&forks[left_fork]);
        printf("Philosopher %d put down left fork (%d).\n", philosopher_id, left_fork);
    }

    return NULL;
}

int main() {
    int i;
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes for forks
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Create philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopher_ids[i]);
    }

    // Wait for philosopher threads to finish (This will never happen)
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }

    return 0;
}
