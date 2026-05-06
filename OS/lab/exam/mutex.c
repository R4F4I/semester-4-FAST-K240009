#include <stdio.h>
#include <pthread.h>
#include <time.h>

#define NUM 5

int shared_data = 0;
pthread_mutex_t mutex;

void *perform_task(void *arg){
    int thread_id = *((int *)arg);
    pthread_mutex_lock(&mutex);
    shared_data++;
    printf("curr: %d, id: %d\n",shared_data,thread_id);
    pthread_mutex_unlock(&mutex);
    pthread_exit(NULL);
}

int main(){

    pthread_t threads[NUM];
    int thread_args[NUM];

    pthread_mutex_init(&mutex,NULL);

    for (size_t i=0; i<NUM; i++) {
        thread_args[i] = i;
        pthread_create(&threads[i],NULL,perform_task,&thread_args[i]);
    }

    for (size_t i=0; i<NUM; i++) {
        pthread_join(threads[i],NULL);
    }

    pthread_mutex_destroy(&mutex);

}