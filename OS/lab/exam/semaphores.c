#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>


sem_t sem_val;

#define NUM 176

void *perform_task(void *arg){

    int thread_id = *((int *)arg);


    sem_wait(&sem_val);
    printf("doing work in thread: %d semaphore: %d \n",thread_id, *((int *) &sem_val));
    sem_post(&sem_val);


    pthread_exit(NULL);
}


int main(){

    pthread_t thread[NUM];

    int id[NUM];

    sem_init(&sem_val,0,3);

    for (int i = 0; i < NUM; i++) {
        id[i]= i;
        pthread_create(&thread[i],NULL,perform_task,&id[i]);
    }
    for (int i = 0; i < NUM; i++) {
        pthread_join(thread[i],NULL);
    }
    sem_destroy(&sem_val);
    return 0;
}