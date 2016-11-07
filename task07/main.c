#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#define num_of_threads 20
#define num_of_iterations 100000000


struct helper {
    int rank;
    double localPi;
};

void* calculate(void *arg) {
    //printf("num: %d\n", *((int*)arg));
    int rank = (*((struct helper*)arg)).rank;
    (*((struct helper*)arg)).localPi = 0;
    int start = rank;
    //int finish = num_of_iterations /  num_of_threads * (rank + 1);
 //   printf("start %d finish %d\n", start, finish);
    int total_iter = 0;
    double localPi = 0;
    double odd;
    for (int i = rank; i < num_of_iterations; i += num_of_threads) {
        if (i % 2 == 0) odd = 1.0;
        else odd = -1.0;
        localPi += odd / (2. * i + 1.);
        total_iter++;
    }
    ((struct helper*)arg)->localPi = localPi;
    printf("local Pi %f in rank %d iter: %d\n", localPi, rank, total_iter);
    return arg;
}

int main() {
    struct helper* help = (struct helper*)malloc(sizeof(struct helper) * num_of_threads);
    double pi = 0;
    int* ranks = (int*)malloc(sizeof(int) * num_of_threads);
    for (int i = 0; i < num_of_threads; i++) {
        help[i].rank = i;
    }
    pthread_t* threads = (pthread_t*)malloc(sizeof(pthread_t) * num_of_threads);
    for (int i = 0; i < num_of_threads; i++)
        pthread_create(&threads[i], NULL, calculate, &help[i]);
    
    struct helper *result;

    for (int i = 0; i < num_of_threads; i++) {
        //         printf("hello3\n");
        pthread_join(threads[i], (void**)&result); 
 //       printf("result: %f\n", result->localPi);
        pi += result->localPi;
    }
    pi *= 4.;
    printf("PI: %f\n", pi);
    free(ranks);
    free(help);
    free(threads);
    return 0;    
}
