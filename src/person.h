#ifndef PERSON_INCLUDED
#define PERSON_INCLUDED

#include <stdlib.h>
#include <unistd.h>

#include "shmem.h"

/// @brief Customer process
/// @param id customer id
/// @param tz maximum time customer will wait
/// @return 0 on success, else 1
int customer(size_t id, size_t tz);

/// @brief Worker process
/// @param id worker id
/// @param tu maximum time worker will take a break
/// @return 0 on sucess, else 1
int worker(size_t id, size_t tu);

/// @brief Waits for random time in interval
/// @param min minimum time to wait
/// @param max maximum time to wait
static inline void person_wait(size_t min, size_t max) {
    // Converts miliseconds to microseconds
    max *= 1000;
    min *= 1000;

    // Waits random number in given interval
    size_t diff = max - min;
    size_t rnd = rand() % diff + min;
    usleep(rnd);
}

#endif // PERSON_INCLUDED
