#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // usleep
#include <time.h>
#include <sys/types.h>
#include <signal.h> // sigwait

#include "person.h"
#include "printer.h"

int customer(size_t id, size_t tz) {
    plog("Z %zu: started\n", id);

    // Random wait time till customer enters post
    srand(time(NULL) + id);
    person_wait(0, tz);

    // Goes home if post is closed
    if (!is_open()) {
        plog("Z %zu: going home\n", id);
        return 0;
    }

    // Enters random queue
    size_t rnd = rand() % QSIZE;
    plog("Z %zu: entering office for a service %d\n", id, rnd + 1);
    queue_enter(rnd, getpid());

    // Waits for signal by worker
    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sig = pthread_sigmask(SIG_BLOCK, &set, NULL);
    if (sig != 0) {
        perr("received invalid signal.\n");
        return 1;
    }

    sigwait(&set, &sig);
    
    // Goes to office worker and then goes home
    plog("Z %zu: called by office worker\n", id);
    person_wait(0, 10);
    plog("Z %zu: going home\n", id);

    return 0;
}
