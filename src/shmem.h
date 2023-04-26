#ifndef SHMEM_INCLUDED
#define SHMEM_INCLUDED

#include <sys/mman.h> // mman
#include <sys/stat.h> //shm
#include <fcntl.h> // shm
#include <semaphore.h> // semaphore
#include <stdbool.h> // bool

typedef struct {
    size_t log_num;
    bool opened;
    sem_t first, second, third;
    sem_t log_num_sem;
} shmem_data;

bool shmem_open(size_t nz);

size_t* get_log_num();

void rel_log_num();

bool is_open();

void close_post();

void shmem_close();

#endif // SHMEM_INCLUDED
