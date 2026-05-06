#define _X_OPEN_SOURCE 700
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void sig_handler(int signum){
    fprintf(stdout,"to exit, press Y: %d\n",signum);
    char choice;
    scanf("%c",&choice);
    if (choice == 'Y') exit(0);
}


int main(){

    if (signal(SIGINT,sig_handler)== SIG_ERR){
        perror("sigaction");
        return EXIT_FAILURE;
    }
    
    // struct sigaction sa;
    
    // sa.sa_handler = sig_handler;
    // sigemptyset(&sa.sa_mask);
    // sa.sa_flags = 0;
    
    
    // if (sigaction(SIGINT,&sa,NULL)== -1){
    //     perror("sigaction");
    //     return EXIT_FAILURE;
    // }


    while (1) {
        sleep(1);
    }

    return 0;

}
