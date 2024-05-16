#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdatomic.h>


#include "lf_queue.h"


typedef struct lf_node {
    int data;
    lf_node *prev;
} lf_node;


lf_queue_t *lf_create_queue() {
    lf_queue_t *queue = malloc(sizeof(lf_queue_t));
    if (queue == NULL) {
        fprintf(stderr, "Error allocating lf_queue_t");
        return NULL;
    }
    queue->back = NULL;
    queue->front = NULL;
    return queue;

}

void lf_destroy_queue(lf_queue_t *queue) {
    lf_node *current = queue->front;
    lf_node *prev = NULL;

    while (current != NULL) {
        prev = current->prev;
        free(current);
        current = prev;
    }
    free(queue);
}


void lf_print_queue(lf_queue_t *queue) {
    if (lf_is_empty(queue)) {
        printf("Queue is empty\n");
    }
    lf_node *current = queue->front;
    while (current != NULL) {
        printf("%d", current->data);
        
        current = current->prev;
        
        if (current != NULL) {
            printf("<-");
        }
    }
    putchar('\n');
}


bool lf_is_empty(lf_queue_t *queue) {
    return __sync_bool_compare_and_swap(&queue->back, NULL, NULL);
}

int lf_enqueue(lf_queue_t *queue, int data) {
    lf_node *new_node = malloc(sizeof(lf_node));
    if (new_node == NULL) {
        fprintf(stderr, "Error allocating new queue node");
        return -1;
    }
    new_node->data = data;
    new_node->prev = NULL;

    lf_node *old_back = NULL;

    if (__sync_bool_compare_and_swap(&queue->back, NULL, new_node)) {
        atomic_store(&queue->front, new_node);
        return 0;
    } 
    
    do { // queue->back is not NULL
        old_back = queue->back;
    // } while(!__sync_bool_compare_and_swap(&queue->back->prev, NULL, new_node));
    } while(!__sync_bool_compare_and_swap(&queue->back, old_back, new_node));
    atomic_store(&old_back->prev, new_node);
    return 0;
}

int lf_dequeue(lf_queue_t *queue, int *data) {
    if (lf_is_empty(queue)) {
        return -1;
    }
    
    lf_node *old_front = NULL;
    
    do {
        old_front = queue->front;
        *data = old_front->data;
    } while (!__sync_bool_compare_and_swap(&queue->front, old_front, old_front->prev));
    
    if (!__sync_bool_compare_and_swap(&queue->back, old_front, NULL)) { // if queue still not empty
        atomic_store(&old_front->prev, NULL);
    }
    
    free(old_front);
    return 0;
}
