// printer.c
// Řešení IOS - Projekt 2
// Autor: Martin Slezák, FIT
// Implementace pomocných printovacích funkcí

#include <stdio.h> // printing
#include <stdarg.h> // n (...) arguments

#include "printer.h"
#include "shmem.h"

int perr(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    fprintf(stderr, "Error: ");
    vfprintf(stderr, fmt, args);
    va_end(args);

    return 1;
}

void plog(const char* fmt, ...) {
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