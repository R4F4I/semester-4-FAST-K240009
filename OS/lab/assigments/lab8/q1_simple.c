#include <stdio.h>
#define SIZE 10000000

float A[SIZE];
float B[SIZE];
float C[SIZE];

void sumArrays(float *A,float *B, float *C){
    for (size_t i = 0; i < SIZE; i++){
        *(C + i) = *(A + i) + *(B + i);
    }
}

int main(){

    for (size_t i = 0; i < SIZE; i++)
    {
        *(A+i) = 1.0;
        *(B+i) = 2.0;
    }
    

    sumArrays(A,B,C);
    printf("example value check at C[10]: %f", *(C+10));
}