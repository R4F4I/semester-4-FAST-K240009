// #include <fcntl.h>
// #include <unistd.h>
// #include <sys/stat.h>
// #include <string.h>

// int main(){
//     int fd;
//     char *fifo = "tmp/fifo";
//     char *msg = "message";

//     mkfifo(fifo,0666);
//     fd = open(fifo,O_WRONLY);
//     write(fd,msg,strlen(msg)+1);
//     close(fd);
// }

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/stat.h>

int main(){
    
    int fd;
    char *fifo = "tmp/fifo";
    char buf[1024];

    fd = open(fifo,O_RDONLY);
    read(fd,buf,sizeof(buf));
    printf("%s",buf);
    close(fd);
}