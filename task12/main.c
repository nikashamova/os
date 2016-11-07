#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
pthread_mutex_t mut;
pthread_cond_t cond;

void* child_body(void* args) {   
     pthread_mutex_lock(&mut);
     pthread_cond_signal(&cond);
     for (int i = 0; i < 10; i++) {
        pthread_cond_wait(&cond, &mut);
        printf("Child is here #%d\n", i);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mut);
}


int main() {
    pthread_mutex_init(&mut, NULL);
    pthread_cond_init(&cond, NULL);
    pthread_t thread;
    pthread_create(&thread, NULL, child_body, NULL);
    pthread_mutex_lock(&mut);
    for (int i = 0; i < 10; i++) {
        pthread_cond_wait(&cond, &mut);
        printf("Parent is here #%d\n", i);
        pthread_cond_signal(&cond);
    }
    pthread_mutex_unlock(&mut);
    pthread_join(thread, NULL);
    return 0;
}
