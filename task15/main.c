#include <stdio.h>
#include <semaphore.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/fcntl.h>
#include <unistd.h>

#define NAME1 "/sem1"
#define NAME2 "/sem2"

int main() {
    sem_t* sem1 = sem_open(NAME1, O_CREAT, S_IRWXU, 0);
    sem_t* sem2 = sem_open(NAME2, O_CREAT, S_IRWXU, 1);

if (fork()) {
        for (int i = 0; i < 10; i++) {
            sem_wait(sem1);
            printf("Parent #%d is here\n", i);
            sem_post(sem2);
        }
    }
    else {
        for (int i = 0; i < 10; i++) {
            sem_wait(sem2);
            printf("Child #%d is here\n", i);
            sem_post(sem1);
        }
    }
    sem_unlink(NAME1);
    sem_unlink(NAME2);
    return 0;
}
