#ifndef SHMEM_INCLUDED
#define SHMEM_INCLUDED

#include <sys/mman.h> // mman
#include <sys/stat.h> //shm
#include <fcntl.h> // shm
#include <semaphore.h> // semaphore
#include <stdbool.h> // bool
#include <signal.h>

#include "queue.h"

#define QSIZE 3

typedef struct {
    size_t log_num;
    sem_t log_num_sem;
    bool opened;
    sem_t queue_sem[QSIZE];
    queue_info_t queue_info[QSIZE];
} shmem_data;

/// @brief Openes and initializes shared memory
/// @param nz number of customers
/// @return 0 on success, else 1
int shmem_open(size_t nz);

/// @brief Initiliazes shared memory
/// @return 0 on success, else 1
int shmem_init(size_t nz);

/// @brief Gets log number and locks log_num_sem semaphore
/// @return log number
size_t* get_log_num(void);

/// @brief Releases log_num_sem semaphore
void rel_log_num(void);

/// @brief Adds customer to the queue
/// @param q queue id
/// @param id id of the customer
void queue_enter(size_t q, pid_t id);

/// @brief Gets queue on given index and locks queue semaphore
/// @param q index of semaphore
/// @return queue on given index
queue_t* get_queue(size_t q);

/// @brief Releases queue semaphore
/// @param q index of semaphore
void rel_queue(size_t q);

/// @brief Checks if post is opened
/// @return true if opened, else false
bool is_open(void);

/// @brief Closes post
void close_post(void);

/// @brief Closes shared memory, destroys semaphores
void shmem_close(void);

/// @brief Destroys queue semaphores
void destroy_queue_sem(void);

/// @brief Destroys first n queue semaphores
/// @param n number to destroy semaphores to
void destroy_queue_sem_n(size_t n);

#endif // SHMEM_INCLUDED
