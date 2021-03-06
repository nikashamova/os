#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int num_of_threads =  5;
int end = 0;
pthread_barrier_t barrier;
pthread_mutex_t mut;
int dif = 10000;
struct helper {
    int rank;
    double localPi;
};

int max = 0;

void* calculate(void *arg) {
    int rank = (*((struct helper*)arg)).rank;
    (*((struct helper*)arg)).localPi = 0;
    double localPi = 0;
    int total_iter = 0;
    int check_num = dif * num_of_threads;
    int i = rank;
    while (end != 1) {
        for (; i < check_num ; i += num_of_threads) {
            total_iter++;
            localPi +=  (((i % 2) == 0)?(1.0):(-1.0)) / (2. * i + 1.);
        }
        check_num += dif * num_of_threads;
    }
    pthread_mutex_lock(&mut);
    if (total_iter >= max) 
        max = total_iter;
    pthread_mutex_unlock(&mut);
    pthread_barrier_wait(&barrier);
    for (; total_iter < max; i += num_of_threads) {
        total_iter++;
        localPi +=  (((i % 2) == 0)?(1.0):(-1.0)) / (2. * i + 1.);
    }
    ((struct helper*)arg)->localPi = localPi;
    printf("local Pi %f in rank %d iter: %d\n", localPi, rank, total_iter);
    return NULL;
}

void handler(int sig) {
    printf("\n-----eeeendn-----\n");
    end = 1;
}



int main(int argc, char *argv[]) {
    if (argc > 1) num_of_threads = atoi(argv[1]);
    pthread_barrier_init(&barrier, NULL, num_of_threads);
    pthread_mutex_init(&mut, NULL);
    struct helper* help = (struct helper*)malloc(sizeof(struct helper) * num_of_threads);
    signal(SIGINT, handler);
    double pi = 0;
    for (int i = 0; i < num_of_threads; i++) {
        help[i].rank = i;
    }
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_of_threads);
    for (int i = 0; i < num_of_threads; i++)
        pthread_create(&threads[i], NULL, calculate, &help[i]);
    

    for (int i = 0; i < num_of_threads; i++) {
        pthread_join(threads[i], NULL); 
        pi += help[i].localPi;
    }
    pi *= 4.;
    printf("PI: %.8f\n", pi);
    pthread_barrier_destroy(&barrier);
    free(help);
    free(threads);
    return 0;    
}
