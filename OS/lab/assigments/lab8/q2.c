#include    <stdio.h>
#include    <stdlib.h>
#include    <pthread.h>

typedef struct
{
    int *arr;
    int size;
} arrBundle;


void *avg(void *args){
    
    arrBundle *arrInt = (arrBundle *)args;
    int sum = 0;
    int size = arrInt->size;
    for (size_t i = 0; i < size; i++)
    {
        sum += arrInt->arr[i];
    }

    int *avg = malloc(sizeof(int));

    *avg = sum/size;
    
    return (void *)avg;

}
void *max(void *args){
    
    arrBundle *arrInt = (arrBundle *)args;

    int *res = malloc(sizeof(int));
    
    int max = -1;
    int size = arrInt->size;
    for (size_t i = 0; i < size; i++){
        if (arrInt->arr[i]>max) max = arrInt->arr[i];
    }
    

    *res = max;

    return (void *)res;
    
     
}

void *min(void *args){
    arrBundle *arrInt = (arrBundle *)args;

    int* res = malloc(sizeof(int));

    int min = 99999;
    int size = arrInt->size;
    for (size_t i = 0; i < size; i++){
        if (arrInt->arr[i] < min) min = arrInt->arr[i];
    }

    *res = min;

    return  (void *)res;
    
}

int main(){
    int numbers[] = {90,81,78,95,79,72,85};
    int size = sizeof(numbers)/sizeof(numbers[0]);

    arrBundle aB;
    aB.arr = numbers;
    aB.size = size;

    pthread_t max_thread, min_thread, avg_thread;
    void  *max_result, *min_result, *avg_result;
    

    pthread_create(&max_thread,NULL,max,&aB);
    pthread_create(&min_thread,NULL,min,&aB);
    pthread_create(&avg_thread,NULL,avg,&aB);

    pthread_join(max_thread,&max_result);
    pthread_join(min_thread,&min_result);
    pthread_join(avg_thread,&avg_result);


    printf("The average value is %d.\n",*(int *)avg_result);
    printf("The minimum value is %d.\n",*(int *)min_result);
    printf("The maximum value is %d.\n",*(int *)max_result);

}