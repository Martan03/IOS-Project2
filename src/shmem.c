#include <stdlib.h> // NULL
#include <unistd.h> // ftruncate, close
#include "shmem.h"

shmem_data* data;

bool shmem_open(size_t nz) {
    int fd = shm_open("/xsleza26-post", O_CREAT | O_RDWR | O_EXCL, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        return false;
    }

    size_t size = sizeof(shmem_data);
    if (ftruncate(fd, size) == -1) {
        close(fd);
        return false;
    }

    data = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    close(fd);

    data->log_num = 1;
    data->opened = true;

    if (sem_init(&data->log_num_sem, 1, 1) == -1) {
        return false;
    }

    return true;
}

bool is_open() {
    return data->opened;
}

size_t* get_log_num() {
    sem_wait(&data->log_num_sem);
    return &data->log_num;
}

void rel_log_num() {
    sem_post(&data->log_num_sem);
}


void close_post() {
    data->opened = false;
}

void shmem_close() {
    sem_destroy(&data->log_num_sem);
    shm_unlink("/xsleza26-post");
}