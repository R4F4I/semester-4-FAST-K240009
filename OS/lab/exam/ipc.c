#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>


// PRODUCER.c

// int main(){
//     const int SIZE = 4096;
//     const char *name = "OS";
//     const char *msg = "hello";

//     int fd;
//     char *ptr;

//     fd = shm_open(name,O_CREAT|O_RDWR, 0666);
//     ftruncate(fd, SIZE);
//     ptr = (char *)mmap(0,SIZE,PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);

//     sprintf(ptr,"%s",msg);

//     return 0;

// }

// CONSUMER.C
// commented parts are producer

int main(){
    const int SIZE = 4096;
    const char *name = "OS";
 // const char *msg = "hello";

    int fd;
    char *ptr;

    fd = shm_open(name,O_RDONLY, 0666);
  //ftruncate(fd, SIZE);
  
    if (fd == -1) exit(-1);
  //ptr = (char *)mmap(0,SIZE,PROT_READ|PROT_WRITE, MAP_SHARED,fd,0);
  //sprintf(ptr,"%s",msg);
    
    ptr = (char *)mmap(0,SIZE,PROT_READ, MAP_SHARED,fd,0);
    printf("%s",ptr);
    shm_unlink(name);

    return 0;

}