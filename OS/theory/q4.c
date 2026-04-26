#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

#define NUM_RUNWAYS 2
#define MAX_FLIGHTS 100
#define THRESHOLD 10 // Seconds before dynamic priority boosts

// --- PROPER DATA STRUCTURES ---

typedef enum { EMERGENCY = 0, LANDING = 1, TAKEOFF = 2, CARGO = 3 } FlightType;

typedef struct {
    int id;
    FlightType type;
    int priority; // Lower integer = Higher priority
    time_t arrival_time;
} Flight;

typedef struct {
    int id;
    int is_busy;
    int preempt_flag;
    Flight current_flight;
} Runway;

// Shared Global State
Flight priority_queue[MAX_FLIGHTS];
int pq_size = 0;
int flight_id_counter = 0;

Runway runway_table[NUM_RUNWAYS];

// --- PTHREAD SYNCHRONIZATION ---
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv_queue = PTHREAD_COND_INITIALIZER;

// Helper: Extract highest priority flight (Simulates Priority Queue Pop)
Flight pop_flight() {
    int best_idx = 0;
    for (int i = 1; i < pq_size; i++) {
        if (priority_queue[i].priority < priority_queue[best_idx].priority) {
            best_idx = i;
        }
    }
    Flight f = priority_queue[best_idx];
    for (int i = best_idx; i < pq_size - 1; i++) {
        priority_queue[i] = priority_queue[i + 1]; // Shift array
    }
    pq_size--;
    return f;
}

// --- THREAD 1: Flight Generator ---
void* generator_thread(void* arg) {
    while (1) {
        sleep(2); // New flight every 2 seconds

        pthread_mutex_lock(&lock);
        if (pq_size < MAX_FLIGHTS) {
            Flight f;
            f.id = ++flight_id_counter;
            f.type = (rand() % 5 == 0) ? EMERGENCY : (rand() % 3 + 1); // 20% Emergency
            f.priority = f.type; // Base priority
            f.arrival_time = time(NULL);

            priority_queue[pq_size++] = f;
            printf("[GENERATOR] Flight %d (Type: %d) added to queue.\n", f.id, f.type);
            
            // WAKE UP RUNWAYS
            pthread_cond_broadcast(&cv_queue);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// --- THREAD 2: Runway Controller ---
void* runway_thread(void* arg) {
    int id = *(int*)arg;
    free(arg);

    while (1) {
        pthread_mutex_lock(&lock);

        // THREAD WAIT: Sleep if priority queue is empty
        while (pq_size == 0) {
            pthread_cond_wait(&cv_queue, &lock);
        }

        // Pop highest priority flight and update Runway Table
        Flight f = pop_flight();
        runway_table[id].is_busy = 1;
        runway_table[id].preempt_flag = 0;
        runway_table[id].current_flight = f;
        
        printf("[RUNWAY %d] Cleared for Flight %d (Prio: %d).\n", id, f.id, f.priority);
        pthread_mutex_unlock(&lock);

        // SIMULATE FLIGHT OPERATION (4 seconds)
        int aborted = 0;
        for (int i = 0; i < 4; i++) {
            sleep(1);
            // Check preemption flag without locking entire system
            if (runway_table[id].preempt_flag == 1) {
                aborted = 1;
                break;
            }
        }

        pthread_mutex_lock(&lock);
        runway_table[id].is_busy = 0; // Mark runway idle
        
        if (aborted) {
            printf("[RUNWAY %d] *** ABORTED Flight %d for Emergency! ***\n", id, f.id);
            priority_queue[pq_size++] = f; // Push aborted flight back to queue
            pthread_cond_broadcast(&cv_queue); // Re-evaluate queue
        } else {
            printf("[RUNWAY %d] Flight %d operation complete.\n", id, f.id);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// --- THREAD 3: Emergency & Priority Monitor ---
void* monitor_thread(void* arg) {
    while (1) {
        sleep(3); // Check system periodically
        pthread_mutex_lock(&lock);
        time_t now = time(NULL);
        int emergency_waiting = 0;

        // 1. DYNAMIC PRIORITY MAP: Boost flights waiting too long
        for (int i = 0; i < pq_size; i++) {
            if (priority_queue[i].type == EMERGENCY) emergency_waiting = 1;
            
            if (priority_queue[i].priority > 0 && difftime(now, priority_queue[i].arrival_time) > THRESHOLD) {
                priority_queue[i].priority--; // Boost priority
                priority_queue[i].arrival_time = now; // Reset timer
                printf("!!! MONITOR: Flight %d priority BOOSTED to %d\n", priority_queue[i].id, priority_queue[i].priority);
            }
        }

        // 2. EMERGENCY PREEMPTION: Check Runway Table
        if (emergency_waiting) {
            for (int i = 0; i < NUM_RUNWAYS; i++) {
                if (runway_table[i].is_busy && runway_table[i].current_flight.type != EMERGENCY) {
                    runway_table[i].preempt_flag = 1; // Signal runway to abort
                    printf("!!! MONITOR: Triggered PREEMPTION on Runway %d\n", i);
                    break; // Preempt one runway for the emergency
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

// --- MAIN SETUP ---
int main() {
    srand(time(NULL));
    pthread_t gen_t, mon_t, r_threads[NUM_RUNWAYS];

    // Initialize Data Structures
    for (int i = 0; i < NUM_RUNWAYS; i++) {
        runway_table[i].id = i;
        runway_table[i].is_busy = 0;
        runway_table[i].preempt_flag = 0;
    }

    // Launch Threads
    pthread_create(&mon_t, NULL, monitor_thread, NULL);
    pthread_create(&gen_t, NULL, generator_thread, NULL);

    for (int i = 0; i < NUM_RUNWAYS; i++) {
        int* id = malloc(sizeof(int));
        *id = i;
        pthread_create(&r_threads[i], NULL, runway_thread, id);
    }

    sleep(40); // Let the simulation run 
    return 0;
}