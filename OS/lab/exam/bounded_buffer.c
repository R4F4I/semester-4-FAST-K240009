#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <semaphore.h>


#define BUFF_size 5

sem_t mutex, empty, full;
int buff[BUFF_size];

int in = 0, out = 0;

void *producer(void *arg){

    int item;

    while (1) {
        item = rand()%100;
        sem_wait(&empty);
        sem_wait(&mutex);
        
        buff[in] = item;
        printf("produced: %d\n",item);
        
        in = (in + 1) % BUFF_size;
        
        sem_post(&mutex);
        sem_post(&full);

        sleep(1);

    }

}

void *consumer(void *arg){

    int item;

    while (1) {
        item = rand()%100;

        sem_wait(&full);
        sem_wait(&mutex);
        
        item = buff[out];
        printf("consumer: %d\n",item);
        
        out = (out + 1) % BUFF_size;
        
        sem_post(&mutex);
        sem_post(&empty);

        sleep(1);

    }

}

int main(){

    pthread_t consumer_t, producer_t;

    sem_init(&mutex,0,1);
    sem_init(&empty,0,BUFF_size);
    sem_init(&full,0,0);
    
    pthread_create(&producer_t,NULL,producer,NULL);
    pthread_create(&consumer_t,NULL,consumer,NULL);
    
    
    pthread_join(producer_t,NULL);
    pthread_join(consumer_t,NULL);
    

    sem_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;


}