#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

#include "mutex_queue.h"


typedef struct mutex_node {
    int data;
    mutex_node *prev;
} mutex_node;


mutex_queue_t *mutex_create_queue() {
    mutex_queue_t *queue = malloc(sizeof(mutex_queue_t));
    if (queue == NULL) {
        fprintf(stderr, "Error allocating mutex_queue_t\n");
        return NULL;
    }

    queue->back = NULL;
    queue->front = NULL;
    pthread_mutex_init(&queue->lock, NULL);
    return queue;
}


void mutex_destroy_queue(mutex_queue_t *queue) {
    mutex_node *current = queue->front;
    mutex_node *prev = NULL;

    while (current != NULL) {
        prev = current->prev;
        free(current);
        current = prev;
    }
    pthread_mutex_destroy(&queue->lock);
    free(queue);
}



void mutex_print_queue(mutex_queue_t *queue) {
    if (lf_is_empty(queue)) {
        printf("Queue is empty\n");
    }
    mutex_node *current = queue->front;
    while (current != NULL) {
        printf("%d", current->data);

        current = current->prev;

        if (current != NULL) {
            printf("<-");
        }
    }
    putchar('\n');
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
    temp_node->prev = NULL;

    if (queue->front == NULL) {
        queue->back = NULL;
    }
    pthread_mutex_unlock(&queue->lock);

    free(temp_node);
    return 0;
}
