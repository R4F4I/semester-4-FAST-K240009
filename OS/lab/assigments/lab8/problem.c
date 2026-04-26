#include <stdio.h>
#include <pthread.h>

#define INTERSECTIONS 5

int traffic_data[INTERSECTIONS] = {450, 120, 890, 340, 560};

typedef struct {
    int index;
    int count;
} Args;

void* get_count(void* arg) {
    Args* a = (Args*)arg;
    a->count = traffic_data[a->index];
    return NULL;
}

int main() {
    pthread_t t[INTERSECTIONS];
    Args a[INTERSECTIONS];
    int total_traffic = 0;

    for (int i = 0; i < INTERSECTIONS; i++) {
        a[i] = (Args){i, 0};
        pthread_create(&t[i], NULL, get_count, &a[i]);
    }

    for (int i = 0; i < INTERSECTIONS; i++) {
        pthread_join(t[i], NULL);
        total_traffic += a[i].count;
    }

    printf("Total city traffic: %d\n", total_traffic);
    return 0;
}