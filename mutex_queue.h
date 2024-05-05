#ifndef MUTEX_QUEUE_H
#define MUTEX_QUEUE_H


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>


// Multithreaded queue using posix mutex


typedef struct mutex_node mutex_node;


typedef struct mutex_queue_t {
    mutex_node *front;
    mutex_node *back;
    pthread_mutex_t lock;
} mutex_queue_t;


mutex_queue_t *mutex_create_queue();

void mutex_destroy_queue(mutex_queue_t *queue);

bool mutex_is_empty(mutex_queue_t *queue);

int mutex_enqueue(mutex_queue_t *queue, int data);

int mutex_dequeue(mutex_queue_t *queue, int *data);


#endif

