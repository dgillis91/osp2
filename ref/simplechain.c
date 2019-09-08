#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define CHILD_PROCESS 0


int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s child_process_count\n", argv[0]);
        return EXIT_FAILURE;
    }

    int child_process_count = atoi(argv[1]);
    pid_t child_pid;

    /* A few things to recall:
    **  (1) In C, 0 is False and everything else is True.
    **  (2) `fork` returns 0 in the child process, and the id
    **      of the child in the parent. 
    ** So, when we call fork, the child does not break, but 
    ** the parent does. This causes the parent to spawn only
    ** one child, but the parent to spawn the rest. 
    */
    int i;
    for (i = 0; i < child_process_count; ++i) {
        if ((child_pid = fork()) != CHILD_PROCESS) {
            break;
        }
    }

    fprintf(stderr, "i: %d process_id: %ld parent_id: %ld child_id: %ld\n",
            i, (long) getpid(), (long) getppid(), (long) child_pid);

    return EXIT_SUCCESS;
}