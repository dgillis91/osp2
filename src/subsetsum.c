/* subsetsum.c
** Program reads a 
*/
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <errno.h>

#include <../include/parse.h>
#include <../include/fileutil.h>
#include <../include/tokenize.h>

#define CHILD_PROCESS 0
#define READ_BUFFER_SIZE 100
#define ERROR_MESSAGE_BUFFER_SIZE 100


void alarm_handler(int);


void error_formatted(char**, const char*, const char*, unsigned int);


void print_error_and_terminate(const char*, const char*);


int main(int argc, char* argv[]) {
    // Parse options.
    program_options_t* program_opts = malloc_default_program_options();
    parse_options(argc, argv, program_opts);

    // Number of child processes to spawn. 
    unsigned int child_process_count = 0;
    // PID of the forked child.
    pid_t child_pid;
    // Error status from `wait()` call.
    int wait_stat = 0;
    // Buffers for reading.
    char* read_buffer = (char*) malloc(sizeof(char) * READ_BUFFER_SIZE);

    // Read and write file descriptors
    int read_fd, write_fd;

    // Add a timer
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        print_error_and_terminate("Failure to set SIGALRM", argv[0]);
    }
    alarm(10);

    // Open the passed in file.
    printf("%s\n", program_opts->input_file);
    if ((read_fd = open(program_opts->input_file, O_RDONLY)) == -1) {
        print_error_and_terminate("Failure to open file", argv[0]);
    }
    printf("Read File Descriptor: %d\n", read_fd);

    // Read the first line
    int bytes_read;
    if ((bytes_read = readline(read_fd, read_buffer, 100)) == 0) {
        print_error_and_terminate("Empty file", argv[0]);
    } else if (bytes_read == -1) {
        print_error_and_terminate("Unable to read", argv[0]);
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
            // Read the line of the file.
            readline(read_fd, read_buffer, READ_BUFFER_SIZE);
            // Get the number of tokens
            int line_token_count = token_count(read_buffer, " ");
            // Create an array to hold them
            int* array = (int*) malloc(sizeof(int) * line_token_count);
            // Tokenize into the array.
            tokenize(read_buffer, array, " ");
            fprintf(stderr, "Line %d: %s\n", i, read_buffer);
            int i;
            for (i = 0; i < line_token_count; ++i) {
                fprintf(stderr, "%d\n", array[i]);
            }
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
    free(read_buffer);
    free_program_options(&program_opts);
}


void alarm_handler(int signum) {
    printf("Alarm raised.\n");
}


void error_formatted(char** buffer, const char* program_name, const char* message, unsigned int message_length) {
    /* Format the error for fprintf. buffer should be long enough to hold message_length + the length of 
    ** the program name.
    */
    unsigned int total_buffer_write_length = message_length + strlen(program_name) + 3;
    snprintf(*buffer, total_buffer_write_length, "%s: %s", program_name, message);
}

void print_error_and_terminate(const char* error_message, const char* script_name) {
    // We need the err number in case malloc sets errno
    int local_errno = errno;
    char* error_message_buffer = (char*) malloc(sizeof(char) * ERROR_MESSAGE_BUFFER_SIZE);
    
    if (error_message_buffer == NULL) {
        perror("Fail to allocate error message buffer");
        errno = local_errno;
        perror("Unable to describe error");
        exit(EXIT_FAILURE);
    }

    error_formatted(&error_message_buffer, script_name, error_message, strlen(error_message));
    perror(error_message_buffer);
    free(error_message_buffer);
    exit(EXIT_FAILURE);
}