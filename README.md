# Bounded Buffer
Implementation of a thread-safe queue with FIFO properties, and stores and returns arbitrary pointers to objects.

## Design Choices
- Circular queue, where the underlying data structure is an array
- Semaphores to manage the queue as a shared resource
- Mutexes to ensure mutual exclusion to help protect the code's critical regions (e.g. pushing and popping)\

**These design choices allow for simple checks of the queue's status:**
- If the queue is full, the program can halt enqueuing elements until there is space in the queue
- If the queue is empty, the program can halt dequeuing elements until there exists an element in the queue

### Circular Queue Struct
The circular queue struct is defined as follows:
```c
typedef struct {
    int front;  // Holds index of the front of the queue (initialized to -1 when queue is empty)
    int rear;   // Holds index of the rear of the queue (initialized to -1 when queue is empty)
    int size;   // Holds the size of the queue
    void **arr; // Array that holds arbitrary pointers to objects
} queue_t;
```

## Files included
    queue {h,c} : header/source file that define the queue API
