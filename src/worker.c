#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <signal.h> // kill
#include <sys/types.h> // pid_t
#include <unistd.h> // usleep

#include "person.h"
#include "printer.h"
#include "queue.h"

int worker(size_t id, size_t tu) {
    plog("U %zu: started\n", id);

    srand(time(NULL) * 5 + id);
    while (1) {
        // Picks random queue
        size_t rnd = rand() % QSIZE;
        queue_t* queue;
        for (size_t i = 0; i < QSIZE; ++i, rnd = (rnd + 1) % QSIZE) {
            queue = get_queue(rnd);
            if (!queue_empty(queue))
                break;
            rel_queue(rnd);
            queue = NULL;
        }

        // Goes home if post is closed and queues are empty
        if (!queue && !is_open()) {
            plog("U %zu: going home\n", id);
            return 0;
        }
        // Takes break if queues are empty
        else if (!queue) {
            plog("U %zu: taking break\n", id);
            person_wait(0, tu);
            plog("U %zu: break finished\n", id);
            continue;
        }

        // Servers the customer
        plog("U %zu: serving a service of type %zu\n", id, rnd + 1);
        pid_t pid = queue_pop(queue);
        rel_queue(rnd);

        kill(pid, SIGUSR1);

        person_wait(0, 10);
        plog("U %zu: service finished\n", id);
    }
}
