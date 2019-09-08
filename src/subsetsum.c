/* subsetsum.c
** Program reads a 
*/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>

#include <../include/parse.h>

#define CHILD_PROCESS 0


void alarm_handler(int);


void error_formatted(char**, char*, const char*, unsigned int);


int main(int argc, char* argv[]) {
    // Parse options
    program_options_t* program_opts = malloc_default_program_options();
    parse_options(argc, argv, program_opts);

    // TODO: This needs to be read from the file.
    unsigned int child_process_count = 2;
    pid_t child_pid;
    int wait_stat = 0;
    char* error_message_buffer = malloc(sizeof(char) * 100);

    // Add a timer
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        char err[] = "Failure to set SIGALRM";
        error_formatted(&error_message_buffer, argv[0], err, strlen(err));
        perror(error_message_buffer);
        return EXIT_FAILURE;
    }
    alarm(10);

    // Open the passed in file.

    // Read the first line

    // Fork off child processes
    int i;
    for (i = 0; i < child_process_count; ++i) {
        if ((child_pid = fork()) == -1) {
            perror("Failed to fork");
            return EXIT_FAILURE;
        }
        
        if (child_pid == CHILD_PROCESS) {
            fprintf(stderr, "i: %d pid: %ld, ppid: %ld cid: %ld\n",
                    i, (long) getpid(), (long) getppid(), (long) child_pid);
            sleep(2);
            break;
        }

        if (child_pid != CHILD_PROCESS) {
            pid_t p = wait(NULL);
            fprintf(stderr, "i: %d pid: %ld ppid: %ld cid: %ld\n",
                    i, (long) getpid(), (long) getppid(), (long) child_pid);
            fprintf(stderr, "Got child: %ld\n", (long) p);
        }
    }

    free_program_options(&program_opts);
}


void alarm_handler(int signum) {
    printf("Alarm raised.\n");
}


void error_formatted(char** buffer, char* program_name, const char* message, unsigned int message_length) {
    /* Format the error for fprintf. buffer should be long enough to hold message_length + the length of 
    ** the program name.
    */
    unsigned int total_buffer_write_length = message_length + strlen(program_name) + 3;
    snprintf(*buffer, total_buffer_write_length, "%s: %s", program_name, message);
}