#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
pthread_mutex_t mutex[3];

struct index {
    int first;
    int second;
};

void* child_body(void* args) {
    struct index index_ch[3] = {{0, 1}, {2, 0}, {1, 2}};
    //usleep(1000);
    int r;
    r = pthread_mutex_lock(&mutex[0]);
    printf("ch +0\n");
    usleep(1000);
    r = pthread_mutex_lock(&mutex[2]);
    printf("ch +2\n");
    for (int i = 0; i < 9; i++) {
        printf("Child is here #%d--------------->\n", i);
        r = pthread_mutex_unlock(&mutex[index_ch[i % 3].first]);
    printf("ch -%d\n", index_ch[i % 3].first);
        r = pthread_mutex_lock(&mutex[index_ch[i % 3].second]);   
    printf("ch +%d\n", index_ch[i % 3].second);
    }
    printf("Child is here #9--------------->\n");
    pthread_mutex_unlock(&mutex[0]);
    printf("ch -0\n");
    pthread_mutex_unlock(&mutex[2]);
    printf("ch -2\n");
    printf("qqqeeee\n");
    pthread_exit(NULL);
}


int main() {
    pthread_mutexattr_t attr;
    pthread_mutexattr_settype(&attr, PTHREAD_MUTEX_ERRORCHECK);
    for (int i = 0; i < 3; i++)
        pthread_mutex_init(&mutex[i], &attr);
    pthread_t thread;
    struct index index_pr[3] = {{2, 0}, {1, 2}, {0, 1}};
    int r;
    pthread_create(&thread, NULL, child_body, NULL);    
    r = pthread_mutex_lock(&mutex[1]);
    printf("pr +1\n");
    r = pthread_mutex_lock(&mutex[2]);
    printf("pr +2\n");
    usleep(1000);
    for (int i = 0; i < 10; i++) {
             printf("Parent is here #%d\n", i);

        r = pthread_mutex_unlock(&mutex[index_pr[i % 3].first]);
    printf("pr -%d\n", index_pr[i % 3].first);
        r = pthread_mutex_lock(&mutex[index_pr[i % 3].second]);   
    printf("pr +%d\n", index_pr[i % 3].second);
   }
   
   pthread_join(thread, NULL);printf("hey\n");
   pthread_mutex_unlock(&mutex[0]);
   printf("pr -0\n");
   pthread_mutex_unlock(&mutex[1]);
   printf("pr -1\n");
   for (int i = 0; i < 3; i ++) {
       pthread_mutex_destroy(&mutex[i]);
   }
   printf("zzz\n");
   return 0;
}
