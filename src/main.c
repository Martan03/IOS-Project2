#include <ctype.h> // isspace
#include <inttypes.h> // SIZE_MAX, fork
#include <stdio.h> // fprintf, stderr
#include <string.h> // strcmp
#include <sys/wait.h> // wait

#include "person.h"
#include "printer.h"
#include "shmem.h"

#define MAX_NZ SIZE_MAX
#define MAX_NU SIZE_MAX
#define MAX_TZ ((size_t) 10000)
#define MAX_TU ((size_t) 100)
#define MAX_F ((size_t) 10000)

/// @brief Prints help
/// @return 0
int help();

/// @brief Parses number from string to size_t, checks max value
/// @param str number in string
/// @param out output size_t variable
/// @param max max value of the number
/// @return true on success, else false
bool parse_number(char* str, size_t* out, size_t max);

int main(int argc, char** argv) {
    if (argc == 2 && strcmp(argv[1], "-c") == 0) {
        shmem_close();
        fprintf(stderr, "Shared memory cleared.\n");
    }

    if (argc == 2 && (strcmp(argv[1], "-h") == 0 ||
        strcmp(argv[1], "--help") == 0)) {
        return help();
    }

    // Checks if all arguments were given
    if (argc != 6) {
        return perr("invalid usage. Type './proj -h' to display help\n");
    }

    size_t nz, nu, tz, tu, f;

    // Checks if arguments are numbers in valid range
    if (!parse_number(argv[1], &nz, MAX_NZ)) {
        return perr("NZ must be positive integer.\n");
    }
    if (!parse_number(argv[2], &nu, MAX_NU)) {
        return perr("NU must be positive integer.\n");
    }
    if (!parse_number(argv[3], &tz, MAX_TZ)) {
        return perr("TZ must be positive integer less than %zu.\n", MAX_TZ);
    }
    if (!parse_number(argv[4], &tu, MAX_TU)) {
        return perr("TU must be positive integer less than %zu.\n", MAX_TU);
    }
    if (!parse_number(argv[5], &f, MAX_F)) {
        return perr("F must be positive integer less than %zu.\n, MAX_F");
    }

    // Opens memory
    if (shmem_open(nz))
        return 1;
        
    // Opens log file
    if (open_log_file())
        return perr("opening log file\n");

    // Creates all processes
    for (size_t i = 1; i <= nz; ++i) {
        if (fork() == 0)
            return customer(i, tz);
    }
    for (size_t i = 1; i <= nu; ++i) {
        if (fork() == 0)
            return worker(i, tu);
    }

    // Waits for random time between f and f / 2 in microseconds
    person_wait(f / 2, f);

    // Closes post
    close_post();
    plog("closing\n");

    // Waits for all child processes
    int status = -1;
    while (wait(&status) != -1)
        ;

    // Destroys semaphores and closes shared memory
    destroy_queue_sem();
    shmem_close();
    close_log_file();

    return 0;
}

int help() {
    printf("Welcome in proj2 (second project for IOS)!\n\n"
           "Usage: ./proj2 NZ NU TZ TZ F\n"
           "  NZ: number of customers\n"
           "  NU: number of office workers\n"
           "  TZ: max. time in miliseconds customer waits to enter post\n"
           "  TU: max. time in miliseconds worker takes a break for\n"
           "   F: max. time in miliseconds after post is closed\n"
    );
    return 0;
}

bool parse_number(char* str, size_t* out, size_t max) {
    // Removes whitespaces from beginning of the string
    while (isspace(*str))
        ++str;

    if (*str == 0 || str[0] == '-')
        return false;

    // Converts string to size_t
    *out = strtoul(str, &str, 10);

    return *str == 0 && *out <= max;
}
