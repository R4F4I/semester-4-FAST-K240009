#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

void *print_primes(void *param) {
    int max = atoi((char *)param);
    for (int i = 2; i <= max; i++) {
        int is_prime = 1;
        for (int j = 2; j * j <= i; j++) {
            if (i % j == 0) {
                is_prime = 0;
                break;
            }
        }
        if (is_prime) {
            printf("%d ", i);
        }
    }
    printf("\n");
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <integer>\n", argv[0]);
        return -1;
    }

    if (atoi(argv[1]) < 2) {
        printf("Please enter a number >= 2.\n");
        return -1;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, print_primes, argv[1]);
    pthread_join(tid, NULL);

    return 0;
}