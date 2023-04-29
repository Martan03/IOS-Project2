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

/// @brief Closes all customers waiting in queue
void kill_customers(void);

/// @brief Waits for all processes to end
void wait_for_end(void);

/// @brief Prints help
/// @return 0
int help(void);

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
        return perr("NZ must be non-negative integer.\n");
    }
    if (!parse_number(argv[2], &nu, MAX_NU) || nu == 0) {
        return perr("NU must be positive integer.\n");
    }
    if (!parse_number(argv[3], &tz, MAX_TZ)) {
        return perr("TZ must be integer in range 0-%zu.\n", MAX_TZ);
    }
    if (!parse_number(argv[4], &tu, MAX_TU)) {
        return perr("TU must be integer in range 0-%zu.\n", MAX_TU);
    }
    if (!parse_number(argv[5], &f, MAX_F)) {
        return perr("F must be integer in range 0-%zu.\n, MAX_F");
    }

    // Opens memory
    if (shmem_open(nz))
        return 1;

    // Opens log file
    if (open_log_file())
        return perr("opening log file\n");

    // Creates worker processes
    for (size_t i = 1; i <= nu; ++i) {
        pid_t fid = fork();
        if (fid == 0)
            return worker(i, tu);
        else if (fid == -1) {
            close_post();
            wait_for_end();
            return perr("creating worker fork (#%zu).\n", i);
        }
    }
    // Creates customer processes
    for (size_t i = 1; i <= nz; ++i) {
        pid_t fid = fork();
        if (fid == 0)
            return customer(i, tz);
        else if (fid == -1) {
            kill_customers();
            close_post();
            wait_for_end();
            return
                perr("creating customer fork (#%zu).\n", i);
        }
    }

    // Waits for random time between f and f / 2 in microseconds
    person_wait(f / 2, f);

    // Closes post
    close_post();
    plog("closing\n");

    wait_for_end();

    return 0;
}

void kill_customers(void) {
    for (size_t i = 0; i < QSIZE; ++i) {
        queue_t* queue = get_queue(i);
        while (!queue_empty(queue)) {
            pid_t pid = queue_pop(queue);
            kill(pid, SIGKILL);
        }
        rel_queue(i);
    }
}

void wait_for_end(void) {
    // Waits for all child processes
    int status = -1;
    while (wait(&status) != -1)
        ;

    // Destroys semaphores and closes shared memory
    destroy_queue_sem();
    shmem_close();
    close_log_file();
}

int help(void) {
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
