#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


int q_critical = 0;
int q_serious = 0;
int q_normal = 0;


pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cv_senior = PTHREAD_COND_INITIALIZER;
pthread_cond_t cv_junior = PTHREAD_COND_INITIALIZER;


void* patient_thread(void* arg) {
    int type = *(int*)arg; 
    free(arg);

    pthread_mutex_lock(&lock);
    
    if (type == 0) {
        q_critical++;
        printf("[+] Critical patient arrived.\n");
        pthread_cond_signal(&cv_senior); 
    } 
    else if (type == 1) {
        q_serious++;
        printf("[+] Serious patient arrived.\n");
        
        
        if (q_serious >= 5) {
            q_serious--;
            q_critical++;
            printf(">>> RULE 3: 5 Serious waiting. Promoted 1 to Critical!\n");
            pthread_cond_signal(&cv_senior);
        } else {
            pthread_cond_broadcast(&cv_senior);
            pthread_cond_broadcast(&cv_junior); 
        }
    } 
    else {
        q_normal++;
        printf("[+] Normal patient arrived.\n");
        pthread_cond_broadcast(&cv_senior);
        pthread_cond_broadcast(&cv_junior);
    }

    pthread_mutex_unlock(&lock);
    return NULL; 
}


void* senior_doc_thread(void* arg) {
    int normal_streak = 0;
    
    while (1) {
        pthread_mutex_lock(&lock);
        
        
        while (q_critical == 0 && q_serious == 0 && q_normal == 0) {
            pthread_cond_wait(&cv_senior, &lock);
        }

        
        if (normal_streak >= 3 && q_serious > 0) {
            q_serious--;
            normal_streak = 0;
            printf("--- RULE 4: Senior doc forced to treat Serious.\n");
        } 
        
        else if (q_critical > 0) {
            q_critical--;
            normal_streak = 0;
            printf("Senior Doc treating Critical.\n");
        } else if (q_serious > 0) {
            q_serious--;
            normal_streak = 0;
            printf("Senior Doc treating Serious.\n");
        } else if (q_normal > 0) {
            q_normal--;
            normal_streak++;
            printf("Senior Doc treating Normal.\n");
        }

        pthread_mutex_unlock(&lock);
        sleep(1); 
    }
    return NULL;
}


void* junior_doc_thread(void* arg) {
    int normal_streak = 0;
    
    while (1) {
        pthread_mutex_lock(&lock);
        
        
        while (q_serious == 0 && q_normal == 0) {
            pthread_cond_wait(&cv_junior, &lock);
        }

        
        if (normal_streak >= 3 && q_serious > 0) {
            q_serious--;
            normal_streak = 0;
            printf("--- RULE 4: Junior doc forced to treat Serious.\n");
        } 
        
        else if (q_serious > 0) {
            q_serious--;
            normal_streak = 0;
            printf("Junior Doc treating Serious.\n");
        } else if (q_normal > 0) {
            q_normal--;
            normal_streak++;
            printf("Junior Doc treating Normal.\n");
        }

        pthread_mutex_unlock(&lock);
        sleep(1); 
    }
    return NULL;
}


void* monitor_thread(void* arg) {
    while (1) {
        sleep(30); 
        
        pthread_mutex_lock(&lock);
        
        if (q_serious > 0) {
            q_serious--;
            q_critical++;
            printf("!!! RULE 5: 30 seconds passed. Promoted Serious to Critical.\n");
            pthread_cond_signal(&cv_senior);
        } else if (q_normal > 0) {
            q_normal--;
            q_serious++;
            printf("!!! RULE 5: 30 seconds passed. Promoted Normal to Serious.\n");
            pthread_cond_broadcast(&cv_junior);
        }
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


int main() {
    pthread_t monitor, s_doc, j_doc;
    
    
    pthread_create(&monitor, NULL, monitor_thread, NULL);
    pthread_create(&s_doc, NULL, senior_doc_thread, NULL);
    pthread_create(&j_doc, NULL, junior_doc_thread, NULL);

    
    for (int i = 0; i < 6; i++) {
        pthread_t p;
        int* type = malloc(sizeof(int));
        *type = 1; 
        pthread_create(&p, NULL, patient_thread, type);
        pthread_detach(p); 
    }

    sleep(40); 
    return 0;
}