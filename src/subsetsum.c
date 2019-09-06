/* subsetsum.c
** Program reads a 
*/
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <../include/parse.h>


void alarm_handler(int);

int main(int argc, char* argv[]) {
    // Parse options
    program_options_t* program_opts = malloc_default_program_options();
    parse_options(argc, argv, program_opts);

    // Add a timer
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Failure to set SIGALRM");
        return EXIT_FAILURE;
    }
    alarm(2);

    free_program_options(&program_opts);
    pause();
}


void alarm_handler(int signum) {
    printf("Alarm raised.\n");
}
