#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "mutex_queue.h"


typedef struct mutex_node {
    int data;
    mutex_node *next;
    mutex_node *prev;
} mutex_node;


mutex_queue_t *mutex_create_queue() {
    mutex_queue_t *queue = malloc(sizeof(mutex_queue_t));
    if (queue == NULL) {
        fprintf(stderr, "Error allocating mutex_queue_t");
        return NULL;
    }

    queue->back = NULL;
    queue->front = NULL;
    pthread_mutex_init(&queue->lock, NULL);
    return queue;
}


void mutex_destroy_queue(mutex_queue_t *queue) {
    // Assumes the thread is not being used here
    mutex_node *current = queue->back;
    mutex_node *next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}


bool mutex_is_empty(mutex_queue_t *queue) {
    pthread_mutex_lock(&queue->lock);
    bool is_empty =  queue->front == NULL;
    pthread_mutex_unlock(&queue->lock);
    return is_empty;
}


int mutex_enqueue(mutex_queue_t *queue, int data) {
    mutex_node *new_node = malloc(sizeof(mutex_node));
    
    if (new_node == NULL) {
        fprintf(stderr, "Error allocating new queue node");
        return -1;
    }

    new_node->prev = NULL;
    new_node->data = data;
    
    pthread_mutex_lock(&queue->lock);
    new_node->next = queue->back;
    
    
    if (queue->front == NULL) {
        queue->back = new_node;
        queue->front = new_node;
    } else {
        queue->back->prev = new_node;
        queue->back = new_node;
    }
    
    pthread_mutex_unlock(&queue->lock);
    
    return 0;
}

int mutex_dequeue(mutex_queue_t *queue, int *data) {
    
    pthread_mutex_lock(&queue->lock);
    
    if (queue->front == NULL) {
        pthread_mutex_unlock(&queue->lock);
        return -1;
    }

    mutex_node *temp_node = queue->front;
    if (data != NULL) {
        *data = temp_node->data;
    }

    queue->front = temp_node->prev;
    if (queue->front == NULL) {
        queue->back = NULL;
    } else {
        queue->front->next = NULL;
    }
    pthread_mutex_unlock(&queue->lock);

    free(temp_node);
    return 0;
}
