/* subsetsum.c
** Program reads a 
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>


void alarm_handler(int);

int main(int argc, char* argv[]) {
    // Parse options

    // Add a timer
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Failure to set SIGALRM");
        return EXIT_FAILURE;
    }
}


void alarm_handler(int signum) {
    printf("Alarm raised.\n");
}
