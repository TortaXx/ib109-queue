#include <math.h>
#include <stdlib.h>
#include <time.h>
#include "mutex_queue.h"
#include "lf_queue.h"

#define THREADS_COUNT 6
#define TASKS_PER_THREAD 50



// ---------------------------------
// FUNCTIONS FOR TESTING MUTEX QUEUE
// ---------------------------------

void enqueue_mutex_thread_fun(void *arg) {
    mutex_queue_t *queue = (mutex_queue_t *) arg;
    
    for (int i = 0; i < TASKS_PER_THREAD; i++) {
        int data = rand() % 100;
        mutex_enqueue(queue, data);
        // printf("Enqueued %d\n", data);
    }
}


void dequeue_mutex_thread_fun(void *arg) {
    mutex_queue_t *queue = (mutex_queue_t *) arg;
    
    if (mutex_is_empty(queue)) {
        // fprintf(stderr, "Can't dequeue, queue is already empty\n");
        return;
    }
    
    for (int i = 0; i < TASKS_PER_THREAD; i++) {
        int data;
        int res = mutex_dequeue(queue, &data);
        
        if (res == -1) {
            // printf("Cant dequeue, queue is empty\n");
        } else {
            // printf("Dequeued %d from queue\n", data);
        }
    }
}


void mutex_thread_fun(void *arg) {
    mutex_queue_t *queue = (mutex_queue_t *) arg;
    
    for (int i = 0; i < TASKS_PER_THREAD; i++) {
        int data = rand() % 100;
        mutex_enqueue(queue, data);
        mutex_dequeue(queue, &data);
    }
}


// -------------------------------------
// FUNCTIONS FOR TESTING LOCK-FREE QUEUE
// -------------------------------------

void enqueue_lf_thread_fun(void *arg) {
    lf_queue_t *queue = (lf_queue_t *) arg;
    
    for (int i = 0; i < 10; i++) {
        int data = rand() % 100;
        lf_enqueue(queue, data);
        // printf("Enqueued %d\n", data);
    }
}


void dequeue_lf_thread_fun(void *arg) {
    lf_queue_t *queue = (lf_queue_t *) arg;

    if (lf_is_empty(queue)) {
        // fprintf(stderr, "Can't dequeue, queue is already empty\n");
        return;
    }
    
    for (int i = 0; i < 10; i++) {
        int data;
        lf_dequeue(queue, &data);
        // printf("Dequeued %d\n", data);
    }
}


void lf_thread_fun(void *arg) {
    lf_queue_t *queue = (lf_queue_t *) arg;
    
    for (int i = 0; i < 10; i++) {
        int data = rand() % 100;
        lf_enqueue(queue, data);
        lf_dequeue(queue, &data);
    }
}



int main(void) {
    pthread_t general_threads[THREADS_COUNT];

    
    // Testing mutex queue
    mutex_queue_t *mutex_queue = mutex_create_queue();
    if (mutex_queue == NULL) {
        return -1;
    }

    clock_t mutex_begin = clock();

    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_create(&general_threads[i], NULL, (void *(*)(void *)) &mutex_thread_fun, mutex_queue);
    }
    // not the best solution
    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_join(general_threads[i], NULL);
    }
    
    clock_t mutex_end = clock();
    
    
    // Testing lock-free queue
    lf_queue_t *lf_queue = lf_create_queue();
    if (lf_queue == NULL) {
        return -1;
    }

    clock_t lf_begin = clock();

    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_create(&general_threads[i], NULL, (void *(*)(void *)) &lf_thread_fun, lf_queue);
    }
    // not the best solution
    for (int i = 0; i < THREADS_COUNT; i++) {
        pthread_join(general_threads[i], NULL);
    }

    clock_t lf_end = clock();

    double mutex_total_time = (double)(mutex_end - mutex_begin);
    double lf_total_time = (double)(lf_end - lf_begin);

    printf("524969\n");
    printf("%d\n", (int) trunc(lf_total_time / mutex_total_time * 100));
    // printf("%f", lf_total_time / mutex_total_time);
}
