#include <stdlib.h>

#include "fifo.h"

fifo_t fifo_init() {
    fifo_t fifo = { .first = NULL, .last = NULL };
    return fifo;
}

void fifo_push(fifo_t* fifo, fifo_item_data_t data) {
    // Creates new item
    fifo_item_t* item = malloc(sizeof(fifo_item_t));
    item->data = data;
    item->prev = NULL;

    // Adds item to fifo
    if (fifo->last) {
        fifo->last->prev = item;
        fifo->last = item;
    } else {
        fifo->last = item;
        fifo->first = item;
    }
}

fifo_item_t* fifo_pop(fifo_t* fifo) {
    if (!fifo->first)
        return NULL;
    fifo_item_t* item = fifo->first;
    fifo->first = fifo->first->prev;

    return item;
}

void fifo_free(fifo_t fifo) {
    fifo_item_t* temp;
    for (fifo_item_t* item = fifo.first; item;) {
        temp = item->prev;
        free(item);
        item = temp;
    }
}
