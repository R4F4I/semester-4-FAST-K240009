#include <pthread.h>
#include <stdio.h>

void *msg(void *ptr){

    char *msg = (char *)ptr;

    printf("%s\n",msg);

    pthread_exit(NULL);



}

int main(){


    pthread_t thread1;

    char *msg1 = "this is a message";

    pthread_create(&thread1,NULL,msg,msg1);
    pthread_join(thread1,NULL);


}
