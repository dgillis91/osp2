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
#include <../include/linked_list.h>

#define CHILD_PROCESS 0
#define READ_BUFFER_SIZE 1000
#define ERROR_MESSAGE_BUFFER_SIZE 100


char* global_script_name = NULL;
int global_read_fd;
int global_write_fd;
pid_t global_child_process = 0;

void alarm_handler(int);
void child_alarm_handler(int);
void child_sigint_handler(int);
void error_formatted(char**, const char*, const char*, unsigned int);
void print_error_and_terminate(const char*, const char*);
void display_subset(int[], int, int, int, long);
void find_subset(int*, int, int, long, int);
int subset_sum(int*, int*, int, int, int, int, int, int, long);


int main(int argc, char* argv[]) {
    // Parse options.
    program_options_t* program_opts = malloc_default_program_options();
    parse_options(argc, argv, program_opts);

    // Point the global script name at argv[0]
    global_script_name = argv[0];

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
    alarm(program_opts->allowable_run_time);

    // Open the passed in file.
    if ((read_fd = open(program_opts->input_file, O_RDONLY)) == -1) {
        print_error_and_terminate("Failure to open file for reading", argv[0]);
    }

    if ((write_fd = open(program_opts->output_file, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR)) == -1) {
        print_error_and_terminate("Failure to open file for writing", argv[0]);
    }
    global_read_fd = read_fd;
    global_write_fd = write_fd;

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
        // Before we fork, we need to set the global 
        // child process id to 0. This will tell the
        // signal handler it doesn't need to kill the
        // child. Note a child proc will never reach
        // this code.
        sigset_t block_set;
        if ((sigemptyset(&block_set) == -1) || (sigaddset(&block_set, SIGALRM) == -1)) {
            print_error_and_terminate("Failure to initialize sigset", argv[0]);
        }
        if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1) {
            print_error_and_terminate("Failure to block SIGALRM", argv[0]);
        }
        global_child_process = 0;
        if (sigprocmask(SIG_UNBLOCK, &block_set, NULL) == -1) {
            print_error_and_terminate("Failure to unblock SIGALRM", argv[0]);
        }

        // Fork off a child
        if ((child_pid = fork()) == -1) {
            perror("Failed to fork");
            return EXIT_FAILURE;
        }
        
        // Set the global child process in the parent and child so we can kill the child
        // in both as necessary.
        if ((sigemptyset(&block_set) == -1) || (sigaddset(&block_set, SIGALRM) == -1)) {
            print_error_and_terminate("Failure to initialize sigset", argv[0]);
        }
        if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1) {
            print_error_and_terminate("Failure to block SIGALRM", argv[0]);
        }
        if (child_pid != CHILD_PROCESS) {
            global_child_process = child_pid;
        } else {
            global_child_process = getpid();
        }
        if (sigprocmask(SIG_UNBLOCK, &block_set, NULL) == -1) {
            print_error_and_terminate("Failure to unblock SIGALRM", argv[0]);
        }

        if (child_pid == CHILD_PROCESS) {
            if (signal(SIGALRM, child_alarm_handler) == SIG_ERR) {
                print_error_and_terminate("Failure to set SIGALRM", argv[0]);
            }
            alarm(1);
            if (signal(SIGINT, child_sigint_handler) == SIG_ERR) {
                print_error_and_terminate("Failure to set SIGINT", argv[0]);
            }
            // Read the line of the file.
            readline(read_fd, read_buffer, READ_BUFFER_SIZE);
            // Get the number of tokens
            int line_token_count = token_count(read_buffer, " ");
            // Create an array to hold them
            int* array = (int*) malloc(sizeof(int) * line_token_count);
            // Tokenize into the array.
            tokenize(read_buffer, array, " ");
            // Number to sum to.
            int sum = array[0];
            // Set to search.
            int* set = (int*) malloc(sizeof(int) * (line_token_count - 1));
            // Copy array.
            int i;
            for (i = 0; i < line_token_count - 1; ++i) {
                set[i] = array[i + 1];
            }
            find_subset(set, line_token_count, sum, (long) getpid(), write_fd);
            // Perform the subset sum problem.
            // Free the allocated set.
            free(array);
            free(set);
            break;
        }

        if (child_pid != CHILD_PROCESS) {
            pid_t p = wait(NULL);
        }
    }

    // Now, at this point, we have gone through all children and don't need to kill
    // the child process. So, we again set the global child to 0. 
    sigset_t block_set;
    if ((sigemptyset(&block_set) == -1) || (sigaddset(&block_set, SIGALRM) == -1)) {
        print_error_and_terminate("Failure to initialize sigset", argv[0]);
    }
    if (sigprocmask(SIG_BLOCK, &block_set, NULL) == -1) {
        print_error_and_terminate("Failure to block SIGALRM", argv[0]);
    }
    global_child_process = 0;
    if (sigprocmask(SIG_UNBLOCK, &block_set, NULL) == -1) {
        print_error_and_terminate("Failure to unblock SIGALRM", argv[0]);
    }

    if (child_pid != CHILD_PROCESS) {
        close(read_fd);
        close(write_fd);
        free(read_buffer);
        free_program_options(&program_opts);
    }
}


void find_subset(int* set, int size, int sum, long pid, int fd) {
    char pid_str_buffer[40];

    int* subset = (int*) malloc(size * sizeof(int));

    if (subset == NULL) {
        perror("Unable to allocate memory");
        return;
    }

    if (!subset_sum(set, subset, size, 0, 0, 0, sum, fd, pid)) {
        sprintf(pid_str_buffer, "%lu: ", pid);
        write(fd, pid_str_buffer, strlen(pid_str_buffer));
        write(fd, "No subset with sum ", strlen("No subset with sum "));
        char buffer[20];
        sprintf(buffer, "%d\n", sum);
        write(fd, buffer, strlen(buffer));
    }

    free(subset);
}


int subset_sum(int* set, int* subset, int n, int subset_size, int total, int node_count, int sum, int fd, long pid) {
    if (total == sum) {
        display_subset(subset, subset_size, sum, fd, pid);
        return 1;
    } else {
        // Iterate over the breadth of the option tree
        int i;
        for (i = node_count; i < n; ++i) {
            subset[subset_size] = set[i];
            if (subset_sum(set, subset, n, subset_size + 1, total + set[i], i + 1, sum, fd, pid) == 1) {
                return 1;
            }
        }
        return 0;
    }
}


void display_subset(int subset[], int length, int sum, int fd, long pid) {
    const int buffer_length = 100;
    char decimal_buffer[40];

    sprintf(decimal_buffer, "%lu: ", pid);
    write(fd, decimal_buffer, strlen(decimal_buffer));

    int i;
    for (i = 0; i < length; ++i) {
        if (i == 0) {
            sprintf(decimal_buffer, "%d", subset[i]);
            write(fd, decimal_buffer, strlen(decimal_buffer));
            strcpy(decimal_buffer, "");
        } else {
            write(fd, " + ", 3);
            sprintf(decimal_buffer, "%d", subset[i]);
            write(fd, decimal_buffer, strlen(decimal_buffer));
            strcpy(decimal_buffer, "");
        }
    }
    sprintf(decimal_buffer, "%d", sum);
    write(fd, " = ", 3);
    write(fd, decimal_buffer, strlen(decimal_buffer));
    write(fd, "\n", 1);
}


void alarm_handler(int signum) {
    int local_errno;
    local_errno = errno;
    if (close(global_read_fd) == -1) {
        perror("Failed to close file in sig handler");
        errno = local_errno;
    }
    if (close(global_write_fd) == -1) {
        perror("Failed to close file in sig handler");
        errno = local_errno;
    }
    if (global_child_process) {
        kill(global_child_process, SIGINT);
    }
    print_error_and_terminate("Failed to complete before time limit.", global_script_name);
}


void child_alarm_handler(int signum) {
    char buffer[100];
    sprintf(buffer, "%lu: No subset found after 1 second.", (long) global_child_process);
    write(global_write_fd, buffer, strlen(buffer));
    kill(global_child_process, SIGKILL);
}


void child_sigint_handler(int signum) {
    exit(EXIT_FAILURE);
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