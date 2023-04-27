#include "logger.h"
#include "shmem.h"

void print_log(const char* fmt, ...) {
    FILE *f = stderr;
    size_t* num = get_log_num();

    va_list args;
    va_start(args, fmt);
    fprintf(f, "%zu: ", *num);
    vfprintf(f, fmt, args);
    va_end(args);

    ++*num;
    rel_log_num();
}