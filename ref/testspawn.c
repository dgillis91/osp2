/* Test to see if parent process waits on a child before exiting. It does not,
** which makes sense. Instead, it becomes an orphan which `init` adopts.
*/
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(void) {
    pid_t child_pid;

    if ((child_pid = fork()) == -1) {
        perror("Failed to fork");
        return EXIT_FAILURE;
    }

    /*In the child process, we get 0 as the `child_pid`*/
    if (child_pid == 0) {
        sleep(15);
        fprintf(stderr, "In child cpid: %ld\n", (long) child_pid);
    } else {
        fprintf(stderr, "In parent cpid: %ld\n", (long) child_pid);
    }

    return EXIT_SUCCESS;
}