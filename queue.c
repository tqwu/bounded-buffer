#include "queue.h"
#include <stdlib.h>
#include <semaphore.h>
#include <pthread.h>

// Defining queue struct
struct queue {
    int front;
    int rear;
    int size;
    void **arr;
};

// Declaring semaphores and mutex
sem_t empty;
sem_t full;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

// Function to create a new queue
queue_t *queue_new(int size) {

    // Dynamically allocate memory for queue struct
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        return NULL;
    }

    // Initializing queue struct
    q->front = -1;
    q->rear = -1;
    q->size = size;
    q->arr = malloc(sizeof(void *) * size);

    // Initializing semaphores
    sem_init(&empty, 0, 0);
    sem_init(&full, 0, size);

    return q;
}

// Function to delete a queue
void queue_delete(queue_t **q) {

    // If attempting to delete a queue that doesn't exist, return
    if (q == NULL || *q == NULL) {
        return;
    }

    // Freeing queue struct
    free((*q)->arr);
    free(*q);
    *q = NULL;

    // Destroying semaphores
    sem_destroy(&empty);
    sem_destroy(&full);
}

// Function to push an element onto the queue
bool queue_push(queue_t *q, void *elem) {

    // If attempting to push to a queue that doesn't exist, return failure
    if (q == NULL) {
        return false;
    }

    // -------------- Critical Region Begins --------------
    // Wait until there is space in the queue
    sem_wait(&full);
    // Acquire mutex
    pthread_mutex_lock(&lock);

    // If the queue is empty, increment front
    if (q->front == -1) {
        q->front += 1;
    }
    // Increment rear (wraps around in circular queue)
    q->rear = (q->rear + 1) % q->size;

    // Push the element to the rear of the queue
    q->arr[q->rear] = elem;

    // Release mutex
    pthread_mutex_unlock(&lock);
    // Atomic increase of the count of empty
    sem_post(&empty);
    // -------------- Critical Region Ends --------------

    return true;
}

// Function to pop an element from the queue
bool queue_pop(queue_t *q, void **elem) {

    // If attempting to pop from a queue that doesn't exist, return failure
    if (q == NULL) {
        return false;
    }

    // -------------- Critical Region Begins --------------
    // Wait until there is an element in the queue
    sem_wait(&empty);
    // Acquire mutex
    pthread_mutex_lock(&lock);

    // Assign elem
    *elem = q->arr[q->front];

    // If popping the only item in the queue, reset front and rear to -1 to mark that the queue is empty
    if (q->front == q->rear) {
        q->front = -1;
        q->rear = -1;
    }
    // Increment front (wraps around in circular queue)
    else {
        q->front = (q->front + 1) % q->size;
    }

    // Release mutex
    pthread_mutex_unlock(&lock);
    // Atomic increase of the count of full
    sem_post(&full);
    // -------------- Critical Region Ends --------------

    // Return success
    return true;
}
