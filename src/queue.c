#include "queue.h"

void queue_push(queue_t* queue, pid_t id) {
    if ((queue->info->end + 1) % queue->info->size == queue->info->start)
        queue->info->start = (queue->info->start + 1) % queue->info->size;

    queue->data[queue->info->end++] = id;
    queue->info->end %= queue->info->size;
}

pid_t queue_pop(queue_t* queue) {
    pid_t item = queue->data[queue->info->start++];
    queue->info->start %= queue->info->size;

    return item;
}

int queue_empty(queue_t* queue) {
    return queue->info->start == queue->info->end;
}
