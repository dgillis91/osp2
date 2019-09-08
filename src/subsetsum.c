/* subsetsum.c
** Program reads a 
*/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>

#include <../include/parse.h>
#include <../include/fileutil.h>

#define CHILD_PROCESS 0
#define READ_BUFFER_SIZE 100
#define ERROR_MESSAGE_BUFFER_SIZE 100


void alarm_handler(int);


void error_formatted(char**, char*, const char*, unsigned int);


int main(int argc, char* argv[]) {
    // Parse options.
    program_options_t* program_opts = malloc_default_program_options();
    parse_options(argc, argv, program_opts);

    // TODO: This needs to be read from the file.
    unsigned int child_process_count = 0;
    // PID of the forked child.
    pid_t child_pid;
    // Error status from `wait()` call.
    int wait_stat = 0;
    // Buffers for error message and reading.
    char* error_message_buffer = malloc(sizeof(char) * ERROR_MESSAGE_BUFFER_SIZE);
    char* read_buffer = (char*) malloc(sizeof(char) * READ_BUFFER_SIZE);

    // Read and write file descriptors
    int read_fd, write_fd;

    // Add a timer
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        char err[] = "Failure to set SIGALRM";
        error_formatted(&error_message_buffer, argv[0], err, strlen(err));
        perror(error_message_buffer);
        return EXIT_FAILURE;
    }
    alarm(10);

    // Open the passed in file.
    printf("%s\n", program_opts->input_file);
    if ((read_fd = open(program_opts->input_file, O_RDONLY)) == -1) {
        char err[] = "Failure to open infile";
        error_formatted(&error_message_buffer, argv[0], err, strlen(err));
        perror(error_message_buffer);
        return EXIT_FAILURE;
    }
    printf("Read File Descriptor: %d\n", read_fd);

    // Read the first line
    int bytes_read;
    if ((bytes_read = readline(read_fd, read_buffer, 100)) == 0) {
        char err[] = "Empty file";
        error_formatted(&error_message_buffer, argv[0], err, strlen(err));
        perror(error_message_buffer);
        return EXIT_FAILURE;
    } else if (bytes_read == -1) {
        char err[] = "Unable to read";
        error_formatted(&error_message_buffer, argv[0], err, strlen(err));
        perror(error_message_buffer);
        return EXIT_FAILURE;
    }
    // Strip the newline.
    read_buffer[bytes_read - 1] = '\0';
    // Get the child process count.
    child_process_count = atoi(read_buffer);

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
            // Don't need to allocate more data for this? I think not, they are
            // all sequential. Is this undefined? 
            readline(read_fd, read_buffer, READ_BUFFER_SIZE);
            fprintf(stderr, "%s\n", read_buffer);
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

    close(read_fd);
    free(error_message_buffer);
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