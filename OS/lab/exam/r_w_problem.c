#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

sem_t rw_mutex, mutex;
int read_count = 0;

void *reader(void *arg){
    while (1) {
        
        // mutex protects read_count
        sem_wait(&mutex);
        read_count++;
        if (read_count==1) sem_wait(&rw_mutex);  // the first reader must lock all 
        sem_post(&mutex);
        
        printf("reading ...\n");
        sleep(1);
        
        sem_wait(&mutex);
        read_count--;
        if (read_count== 0) sem_post(&rw_mutex);
        sem_post(&mutex);
        sleep(1);
    }
    return NULL;
}

void *writer(void *arg){
    while (1) {
        sem_wait(&rw_mutex);
        
        printf("writing ...\n");
        sleep(2);
        
        sem_post(&rw_mutex);
        sleep(2);
    }
    return NULL;
}

int main(){
    pthread_t r[2], w[2];
    sem_init(&rw_mutex, 0, 1);
    sem_init(&mutex, 0, 1);
    
    for (int i = 0; i<2; i++) {
        pthread_create(&r[i], NULL, reader, (void *)i);
    }
    for (int i = 0; i<2; i++) {
        pthread_create(&w[i], NULL, writer, (void *)i);
    }
    
    
    for (int i = 0; i<2; i++) pthread_join(r[i], NULL);
    for (int i = 0; i<2; i++) pthread_join(w[i], NULL);

    sem_destroy(&rw_mutex);
    sem_destroy(&mutex);
    
}