#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>

sem_t sem1;
sem_t sem2;

void* print(void *arg) {
    for (int i = 0; i < 10; i++) {
        sem_wait(&sem2);
        printf("Child #%d is here\n", i);
        sem_post(&sem1);
    }
}

int main() {
    pthread_t thr;
    pthread_create(&thr, NULL, print, NULL);
    sem_init(&sem1, 0, 0);
    sem_init(&sem2, 0, 1);
    for (int i = 0; i < 10; i++) {
        sem_wait(&sem1);
        printf("Parent #%d is here\n", i);
        sem_post(&sem2);
    }
    
    return 0;
}
