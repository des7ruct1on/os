#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdbool.h>

#define NUM_PHILOSOPHERS 5

pthread_t philosophers[NUM_PHILOSOPHERS];
pthread_mutex_t forks[NUM_PHILOSOPHERS];
sem_t semaphore;

void *philosopher(void *id) {
    int philosopher_id = *((int *)id);
    int left_fork = philosopher_id;
    int right_fork = (philosopher_id + 1) % NUM_PHILOSOPHERS;

    while (true) {
        // Thinking
        printf("Philosopher %d is thinking.\n", philosopher_id);
        usleep(1000000); // Sleep for 1 second

        // Wait for available forks
        sem_wait(&semaphore);

        // Pick up left fork
        pthread_mutex_lock(&forks[left_fork]);
        printf("Philosopher %d picked up left fork (%d).\n", philosopher_id, left_fork);

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

        // Signal that forks are available
        sem_post(&semaphore);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    int i;
    int philosopher_ids[NUM_PHILOSOPHERS];

    // Initialize mutexes for forks
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_init(&forks[i], NULL);
    }

    // Initialize semaphore
    sem_init(&semaphore, 0, NUM_PHILOSOPHERS - 1);

    // Create philosopher threads
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        philosopher_ids[i] = i;
        pthread_create(&philosophers[i], NULL, philosopher, (void *)&philosopher_ids[i]);
    }

    // Wait for philosopher threads to finish
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(philosophers[i], NULL);
    }

    // Destroy mutexes and semaphore
    for (i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_mutex_destroy(&forks[i]);
    }
    sem_destroy(&semaphore);

    return 0;
}
