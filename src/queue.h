#ifndef QUEUE_INCLUDED
#define QUEUE_INCLUDED

#include <stdlib.h> // size_t
#include <sys/types.h> // pid_t

typedef struct {
    size_t start;
    size_t end;
    size_t size;
} queue_info_t;

typedef struct {
    queue_info_t* info;
    pid_t* data;
} queue_t;

/// @brief Pushes item to the queue
/// @param queue queue struct
/// @param id data to be pushed
void queue_push(queue_t* queue, pid_t id);

/// @brief Pops item from the queue
/// @param queue queue struct
/// @return poppped item
pid_t queue_pop(queue_t* queue);

/// @brief Checks if queue is empty
/// @param queue queue to be checked
/// @return true if empty, else false
int queue_empty(queue_t* queue);

#endif // QUEUE_INCLUDED
