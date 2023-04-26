#ifndef FIFO_INCLUDED
#define FIFO_INCLUDED

typedef int fifo_item_data_t;
typedef struct fifo_item_t fifo_item_t;

/// @brief Fifo item containing data and pointer to previous item
struct fifo_item_t {
    fifo_item_data_t data;
    fifo_item_t* prev;
};

/// @brief Fifo struct containing pointer to first and last element
typedef struct {
    fifo_item_t* first;
    fifo_item_t* last; 
} fifo_t;

/// @brief Initiliazies fifo
/// @return created fifo
fifo_t fifo_init();

/// @brief Pushes item to fifo
/// @param fifo queue struct
/// @param item item to be pushed
void fifo_push(fifo_t* fifo, fifo_item_data_t data);

/// @brief Pops item from fifo and returns it
/// @param fifo queue struct
/// @return popped item
fifo_item_t* fifo_pop(fifo_t* fifo);

/// @brief Frees fifo
/// @param fifo queue struct
void fifo_free(fifo_t fifo);

#endif // FIFO_INCLUDED
