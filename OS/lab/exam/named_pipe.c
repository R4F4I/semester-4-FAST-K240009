#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>

// PRD
int main(){
    int fd;
    char *fifo = "tmp/myfifo";
    char *msg = "data via fifo";

    mkfifo(fifo,0666);
    fd = open(fifo,O_WRONLY);
    write(fd,msg,strlen(msg)+1);
    close(fd);

    return 0;

}

// CONS
// int main(){
//     int fd;
//     char *fifo = "tmp/myfifo";
//     char buf[1024];

//  // mkfifo(fifo,0666);
//  // fd = open(fifo,O_WRONLY);
//  // write(fd,msg,strlen(msg)+1);
    
//     fd = open(fifo,O_RDONLY);
//     read(fd,buf,sizeof(buf));
//     printf("%s",buf);
//     close(fd);

//     return 0;
// }

