#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int eventAvailable = 0;

void* producer(void* arg) {
    while (1) {
        sleep(1);
        pthread_mutex_lock(&lock);
        eventAvailable++;
        printf("Event produced. Current event count: %d\n", eventAvailable);
        pthread_cond_signal(&cond); 
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}

void* consumer(void* arg) {
    while (1) {
        pthread_mutex_lock(&lock);
        while (eventAvailable == 0) {
            pthread_cond_wait(&cond, &lock);
        }
        eventAvailable--;
        printf("Event consumed. Current event count: %d\n", eventAvailable);
        pthread_mutex_unlock(&lock);
    }
    return NULL;
}


int main() {
    pthread_t producerThread, consumerThread;

    pthread_create(&producerThread, NULL, producer, NULL);
    pthread_create(&consumerThread, NULL, consumer, NULL);
    
    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);
    
    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond);

    return 0;
}