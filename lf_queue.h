#ifndef LF_QUEUE_H
#define LF_QUEUE_H


#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>


// Multithreaded lock-free queue using the gcc builtin `__sync_bool_compare_and_swap gcc` 


typedef struct lf_node lf_node;


typedef struct lf_queue_t {
    lf_node *front;
    lf_node *back;
} lf_queue_t;


lf_queue_t *lf_create_queue();

void lf_destroy_queue(lf_queue_t *queue);

bool lf_is_empty(lf_queue_t *queue);

int lf_enqueue(lf_queue_t *queue, int data);

int lf_dequeue(lf_queue_t *queue, int *data);

void lf_print_queue(lf_queue_t *queue);


#endif

