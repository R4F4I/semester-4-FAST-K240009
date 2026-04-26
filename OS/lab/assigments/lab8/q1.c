#include    <stdio.h>
#include    <pthread.h>

#define     SIZE        10000000
#define     N_THREADS   10


float A[SIZE];
float B[SIZE];
float C[SIZE];

typedef struct 
{
    int startIndex, endIndex;
} threadArgs;


void *sumArraysConc(void *Args){

    threadArgs *args = (threadArgs *)Args;

    int start = args->startIndex;
    int end = args->endIndex;

    for (size_t i = start; i < end; i++){
        *(C + i) = *(A + i) + *(B + i);
    }

    return NULL;
}

int main(){

    for (size_t i = 0; i < SIZE; i++)
    {
        *(A+i) = 1.0;
        *(B+i) = 2.0;
    }
    
    pthread_t threads[N_THREADS];
    threadArgs T_args[N_THREADS];

    size_t chunkSize = SIZE/N_THREADS;

    for (size_t i = 0; i < N_THREADS; i++)
    {
        T_args[i].startIndex = i*chunkSize;
        
        //  for the last chunk to take the rest
        if (i==N_THREADS-1)
        {
            T_args[i].endIndex = SIZE;
            
        }
        else
        {
            T_args[i].endIndex = (i+1)*chunkSize;
            
        }
        
        pthread_create(&threads[i],NULL,sumArraysConc,&T_args[i]);
    }
    
    for (size_t i = 0; i < N_THREADS; i++)
    {
        pthread_join(threads[i],NULL);
    }
    



    
    printf("example value check at C[10] done by thread 0:      %f\n", *(C+10));
    printf("example value check at C[5500000] done by thread 5: %f\n", *(C+5500000));
    printf("example value check at C[9999999] done by thread 9: %f\n", *(C+9999999));
}