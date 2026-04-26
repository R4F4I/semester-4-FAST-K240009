#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

#define N 1000
#define M 4
#define T_HOT 35.0
#define T_COLD -10.0
#define TILE_SIZE 100
#define NUM_THREADS 8
#define NAN_VAL -999.0

double sat[M][N][N];
double global[N][N];
double norm_mat[N][N];
double risk[N][N];
int is_hot[N][N] = {0};
int is_cold[N][N] = {0};

double g_max = -1e9;
double g_min = 1e9;
double g_mean = 0;
double g_variance = 0;
int g_anomalies = 0;
int g_hotspots = 0;
int g_coldspots = 0;

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
int current_tile = 0;
int total_tiles = (N / TILE_SIZE) * (N / TILE_SIZE);

void* prep_satellite(void* arg) {
    long id = (long)arg;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (sat[id][i][j] == NAN_VAL) {
                double sum = 0; int c = 0;
                if (i > 0 && sat[id][i-1][j] != NAN_VAL) { sum += sat[id][i-1][j]; c++; }
                if (i < N-1 && sat[id][i+1][j] != NAN_VAL) { sum += sat[id][i+1][j]; c++; }
                if (j > 0 && sat[id][i][j-1] != NAN_VAL) { sum += sat[id][i][j-1]; c++; }
                if (j < N-1 && sat[id][i][j+1] != NAN_VAL) { sum += sat[id][i][j+1]; c++; }
                sat[id][i][j] = c > 0 ? sum / c : 0.0;
            }
        }
    }
    return NULL;
}

void* merge_regions(void* arg) {
    long id = (long)arg;
    int chunk = N / NUM_THREADS;
    int start = id * chunk;
    int end = (id == NUM_THREADS - 1) ? N : start + chunk;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            double sum = 0;
            for (int k = 0; k < M; k++) sum += sat[k][i][j];
            global[i][j] = sum / M;
        }
    }
    return NULL;
}

void* process_tiles(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        if (current_tile >= total_tiles) {
            pthread_mutex_unlock(&lock);
            break;
        }
        int tile = current_tile++;
        pthread_mutex_unlock(&lock);

        int tiles_per_row = N / TILE_SIZE;
        int row_start = (tile / tiles_per_row) * TILE_SIZE;
        int col_start = (tile % tiles_per_row) * TILE_SIZE;

        double l_max = -1e9, l_min = 1e9, l_sum = 0;
        int count = TILE_SIZE * TILE_SIZE;

        for (int i = row_start; i < row_start + TILE_SIZE; i++) {
            for (int j = col_start; j < col_start + TILE_SIZE; j++) {
                double val = global[i][j];
                if (val > l_max) l_max = val;
                if (val < l_min) l_min = val;
                l_sum += val;
            }
        }
        
        double l_mean = l_sum / count;
        double l_var_sum = 0;
        for (int i = row_start; i < row_start + TILE_SIZE; i++) {
            for (int j = col_start; j < col_start + TILE_SIZE; j++) {
                l_var_sum += (global[i][j] - l_mean) * (global[i][j] - l_mean);
            }
        }
        double l_stddev = sqrt(l_var_sum / count);
        int l_anomalies = 0;

        for (int i = row_start; i < row_start + TILE_SIZE; i++) {
            for (int j = col_start; j < col_start + TILE_SIZE; j++) {
                if (fabs(global[i][j] - l_mean) > 2 * l_stddev) l_anomalies++;
            }
        }

        pthread_mutex_lock(&lock);
        if (l_max > g_max) g_max = l_max;
        if (l_min < g_min) g_min = l_min;
        g_mean += l_sum;
        g_variance += l_var_sum;
        g_anomalies += l_anomalies;
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* task_A_hotspots(void* arg) {
    int l_hot = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (global[i][j] > T_HOT) {
                is_hot[i][j] = 1;
                l_hot++;
            }
        }
    }
    pthread_mutex_lock(&lock);
    g_hotspots += l_hot;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* task_B_coldspots(void* arg) {
    int l_cold = 0;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (global[i][j] < T_COLD) {
                is_cold[i][j] = 1;
                l_cold++;
            }
        }
    }
    pthread_mutex_lock(&lock);
    g_coldspots += l_cold;
    pthread_mutex_unlock(&lock);
    return NULL;
}

void* task_C_normalize(void* arg) {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            norm_mat[i][j] = (global[i][j] - g_min) / (g_max - g_min);
        }
    }
    return NULL;
}

void* task_D_risk_scores(void* arg) {
    long id = (long)arg;
    int chunk = N / NUM_THREADS;
    int start = id * chunk;
    int end = (id == NUM_THREADS - 1) ? N : start + chunk;

    for (int i = start; i < end; i++) {
        for (int j = 0; j < N; j++) {
            int prox_hot = 0, prox_cold = 0;
            for (int di = -2; di <= 2; di++) {
                for (int dj = -2; dj <= 2; dj++) {
                    if (abs(di) + abs(dj) <= 2) {
                        int ni = i + di, nj = j + dj;
                        if (ni >= 0 && ni < N && nj >= 0 && nj < N) {
                            if (is_hot[ni][nj]) prox_hot++;
                            if (is_cold[ni][nj]) prox_cold++;
                        }
                    }
                }
            }
            if (prox_hot > 0 && prox_cold > 0) norm_mat[i][j] *= 1.1;
            risk[i][j] = (norm_mat[i][j] * prox_hot) / (prox_cold + 1.0);
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));
    for (int k = 0; k < M; k++) {
        for (int i = 0; i < N; i++) {
            for (int j = 0; j < N; j++) {
                // 10% chance of a missing reading (NaN)
                if (rand() % 10 == 0) {
                    sat[k][i][j] = NAN_VAL;
                } else {
                    // Random temp between -20.0 and 50.0
                    sat[k][i][j] = -20.0 + ((double)(rand() % 7000) / 100.0);
                }
            }
        }
    }
    pthread_t threads[NUM_THREADS];

    for (long i = 0; i < M; i++) {
        pthread_create(&threads[i], NULL, prep_satellite, (void*)i);
    }
    for (long i = 0; i < M; i++) pthread_join(threads[i], NULL);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, merge_regions, (void*)i);
    }
    for (long i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, process_tiles, NULL);
    }
    for (long i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);

    g_mean /= (N * N);
    g_variance /= (N * N);

    pthread_t tA, tB, tC;
    pthread_create(&tA, NULL, task_A_hotspots, NULL);
    pthread_create(&tB, NULL, task_B_coldspots, NULL);
    pthread_join(tA, NULL);
    pthread_join(tB, NULL);
    
    pthread_create(&tC, NULL, task_C_normalize, NULL);
    pthread_join(tC, NULL);

    for (long i = 0; i < NUM_THREADS; i++) {
        pthread_create(&threads[i], NULL, task_D_risk_scores, (void*)i);
    }
    for (long i = 0; i < NUM_THREADS; i++) pthread_join(threads[i], NULL);

    printf("Global Max: %f, Min: %f, Mean: %f, Variance: %f\n", g_max, g_min, g_mean, g_variance);
    printf("Hotspots: %d, Coldspots: %d, Anomalies: %d\n", g_hotspots, g_coldspots, g_anomalies);

    printf("Sample 5x5 Normalized Matrix:\n");
    for(int i=0; i<5; i++){
        for(int j=0; j<5; j++){
            printf("%.2f ", norm_mat[i][j]);
        }
        printf("\n");
    }

    return 0;
}