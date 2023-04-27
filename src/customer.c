#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <time.h>
#include <sys/types.h>
#include <signal.h> // sigwait

#include "customer.h"
#include "logger.h"

int customer(size_t id, size_t tz) {
    print_log("Z %zu: started\n", id);

    // Random wait time till customer enters post
    srand(time(NULL) + id);
    size_t rnd = rand() % tz * 1000;
    usleep(rnd);

    if (!is_open()) {
        print_log("Z %zu: going home\n", id);
        return 0;
    }

    rnd = rand() % QSIZE;
    print_log("Z %zu: entering office for a service %d\n", id, rnd + 1);
    queue_enter(rnd, getpid());

    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);

    sig = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (sig != 0) {
        return 1;
    }

    sigwait(&set, &sig);
    
    print_log("Z %zu: called by office worker\n", id);
    rnd = rand() % 10 * 1000;
    usleep(rnd);
    print_log("Z %zu: going home\n", id);

    return 0;
}
