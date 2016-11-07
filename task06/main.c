#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

void msg_printer(void *arg) {
    printf("\nC A N C E L\n");
}

void* thread_printer(void *arg) {
    pthread_cleanup_push(msg_printer, NULL);
    while (1) {
        printf("To infinity and beyond \n");
    }
    pthread_cleanup_pop(1);

}

int main() {
    pthread_t thread;
    int code;
    //int i;

    code = pthread_create(&thread, NULL, thread_printer, NULL);
    if (code != 0) { // error case
        printf("creating thread error\n");
        return 1;
    }
    sleep(2);
    printf("Time for cancel\n");
    code = pthread_cancel(thread);
    if (code != 0) { // error case
        printf("canceling thread error\n");
        return 1;
    }
    int result = 0;
    int retval = 0;
    result = pthread_join(thread, (void**)&retval);
    if (result != 0)
        printf("cannot join\n"); 
 //   printf("\ncancel %d retval %d\n", PTHREAD_CANCELED, retval);
    pthread_exit(NULL);
}
