#include <stdlib.h> // NULL
#include <unistd.h> // ftruncate, close
#include "shmem.h"

shmem_data* data;
queue_t queue[QSIZE];

bool shmem_open(size_t nz) {
    // Opens shared memory
    int fd = shm_open(
        "/xsleza26-post",
        O_CREAT | O_RDWR | O_EXCL,
        S_IRUSR | S_IWUSR
    );
    if (fd == -1) {
        return false;
    }

    // Truncates shared memory
    ++nz;
    size_t size = sizeof(shmem_data) + QSIZE * nz * sizeof(pid_t);
    if (ftruncate(fd, size) == -1) {
        close(fd);
        return false;
    }

    // Maps shared memory
    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    shmem_init(nz);

    return true;
}

bool shmem_init(size_t nz) {
    // Initializes queues
    for (size_t i = 0; i < QSIZE; ++i) {
        queue[i].data = (pid_t*) ((char*) data + sizeof(*data) + i * nz * sizeof(pid_t));
        data->queue_info[i].start = 0;
        data->queue_info[i].end = 0;
        data->queue_info[i].size = nz;
        queue[i].info = &data->queue_info[i];
    }

    data->log_num = 1;
    if (sem_init(&data->log_num_sem, 1, 1) == -1) {
        return false;
    }

    for (size_t i = 0; i < QSIZE; ++i) {
        if (sem_init(&data->queue_sem[i], 1, 1) == -1) {
            destroy_queue_sem_n(i - 1);
            return false; 
        }
    }

    data->opened = true;

    return true;
}

size_t* get_log_num() {
    sem_wait(&data->log_num_sem);
    return &data->log_num;
}

void rel_log_num() {
    sem_post(&data->log_num_sem);
}

void queue_enter(size_t q, pid_t id) {
    sem_wait(&data->queue_sem[q]);
    queue_push(&queue[q], id);
    sem_post(&data->queue_sem[q]);
}

queue_t* get_queue(size_t q) {
    sem_wait(&data->queue_sem[q]);
    return &queue[q];
}

void rel_queue(size_t q) {
    sem_post(&data->queue_sem[q]);
}

bool is_open() {
    return data->opened;
}

void close_post() {
    data->opened = false;
}

void shmem_close() {
    sem_destroy(&data->log_num_sem);
    shm_unlink("/xsleza26-post");
}

void destroy_queue_sem() {
    destroy_queue_sem_n(QSIZE);
}

void destroy_queue_sem_n(size_t n) {
    for (size_t i = 0; i < n || i < QSIZE; ++i) {
        sem_destroy(&data->queue_sem[i]);
    }
}
