#include <stdlib.h> // rand, srand
#include <time.h> // time
#include <signal.h> // kill
#include <sys/types.h> // pid_t
#include <unistd.h> // usleep

#include "worker.h"
#include "logger.h"
#include "queue.h"

int worker(size_t id, size_t tu) {
    print_log("U %zu: started\n", id);

    srand(time(NULL) - id);
    while (1) {
        size_t rnd = rand() % QSIZE;
        queue_t* queue = get_queue(rnd);

        if (queue_empty(queue)) {
            rel_queue(rnd);
            rnd = (rnd + 1) % QSIZE;

            for (size_t i = 1; i < QSIZE; ++i, rnd = (rnd + 1) % QSIZE) {
                queue = get_queue(rnd);
                if (!queue_empty(queue))
                    break;
                rel_queue(rnd % QSIZE);
                queue = NULL;
            }
        }

        if (!queue && !is_open()) {
            print_log("U %zu: going home\n", id);
            return 0;
        }

        else if (!queue) {
            print_log("U %zu: taking break\n", id);
            rnd = rand() % tu * 1000;
            usleep(rnd);
            print_log("U %zu: break finished\n", id);
            continue;
        }

        print_log("U %zu: serving a service of type %zu\n", id, rnd + 1);
        pid_t pid = queue_pop(queue);
        rel_queue(rnd);

        kill(pid, SIGUSR1);

        rnd = rand() % 10 * 1000;
        usleep(rnd);
        print_log("U %zu: service finished\n", id);
    }
}
