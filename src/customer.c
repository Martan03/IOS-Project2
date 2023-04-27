#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <time.h>
#include <sys/types.h>

#include "customer.h"
#include "logger.h"

#define QSIZE 3

int customer(size_t id, size_t tz) {
    print_log("Z %zu: started\n", id);

    // Random wait time till customer enters post
    srand(time(NULL) + id);
    size_t rnd = rand() % tz * 1000;
    usleep(rnd);

    if (!is_open()) {
        print_log("Z %zu: going home\n", id);
        return 1;
    }

    rnd = rand() % QSIZE;
    print_log("Z %zu: entering office for a service %d\n", id, rnd + 1);
    queue_enter(rnd, getpid());

    return 0;
}
