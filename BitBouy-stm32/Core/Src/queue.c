// queue.c
#include "queue.h"

void init_queue(Queue *q) {
    q->front = 0;
    q->rear = -1;
    q->count = 0;
}

bool is_queue_full(Queue *q) {
    return q->count == QUEUE_SIZE;
}

bool is_queue_empty(Queue *q) {
    return q->count == 0;
}

bool enqueue(Queue *q, const char *message) {
    if (is_queue_full(q)) {
        return false;
    }
    q->rear = (q->rear + 1) % QUEUE_SIZE;
    strncpy(q->data[q->rear], message, MESSAGE_MAX_LENGTH - 1);
    q->data[q->rear][MESSAGE_MAX_LENGTH - 1] = '\0';
    q->count++;
    return true;
}

bool dequeue(Queue *q, char *message_out) {
    if (is_queue_empty(q)) {
        return false;
    }
    strncpy(message_out, q->data[q->front], MESSAGE_MAX_LENGTH);
    q->front = (q->front + 1) % QUEUE_SIZE;
    q->count--;
    return true;
}
