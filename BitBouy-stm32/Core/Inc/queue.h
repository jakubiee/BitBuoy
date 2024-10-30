#ifndef QUEUE_H
#define QUEUE_H

#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#define QUEUE_SIZE 10
#define MESSAGE_MAX_LENGTH 50

typedef struct {
    char data[QUEUE_SIZE][MESSAGE_MAX_LENGTH];
    int front;
    int rear;
    int count;
} Queue;


void init_queue(Queue *q);

bool is_queue_full(Queue *q);

bool is_queue_empty(Queue *q);

bool enqueue(Queue *q, const char *message);

bool dequeue(Queue *q, char *message_out);

#endif
