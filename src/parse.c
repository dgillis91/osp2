#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "../include/parse.h"


void print_help_and_terminate(char* argv[]) {
    printf("Program to compute the subset sum problem of n many\n"
           "lines of a file. The first line of the file indicates\n"
           "the number of lines to sum.\n");
    printf("Usage: %s [-h] [-t -i -o] [dirname]\n"
           "For: \n"
           "    -h: Display help and exit.\n"
           "    -t: Amount of time to allow the program to run for\n"
           "    -i: inputfilename\n"
           "    -o: outputfilename\n", argv[0]);
    exit(EXIT_SUCCESS);
}


program_options_t* malloc_default_program_options() {
    /* Allocate the struct pointer and set its attributes to NULL. I'm
    ** not going to allocate the memory for the file paths here.
    */
    program_options_t* program_opts = (program_options_t*) malloc(sizeof(program_options_t));
    // Recall that malloc return null for failure and sets errno to ENOMEM.
    if (program_opts == NULL) {
        return NULL;
    }
    program_opts->input_file = program_opts->output_file = NULL;
    program_opts->allowable_run_time = 10;
    return program_opts;
}

void free_program_options(program_options_t** program_opts) {
    /* Because the options are going to be dynamically allocated,
    ** and they have a dynamically allocated 'string', we create
    ** a method to clean up the allocated memory. 
    ** @Param:
    **  - program_opts: pointer to a program_options structure.
    */
    if ((*program_opts)->output_file != NULL) {
        free((*program_opts)->output_file);
    }
    if ((*program_opts)->input_file != NULL) {
        free((*program_opts)->input_file);
    }
    (*program_opts)->input_file = (*program_opts)->output_file = NULL;
    *program_opts = NULL;
}


void parse_options(int argc, char* argv[], program_options_t* program_opts) {
    /* Parse the command line options and modify the passed in `program_opts`
     * with them. 
     * @Params:
     * * Note argc and argv come from the main method.
     * - argc: Count of arguments from the program.
     * - argv: Vector of arguments.
     * - program_opts: Pointer to a program_options_t. Is modified.
     * @Reference: 
     * - https://www.gnu.org/software/libc/manual/html_node/Example-of-Getopt.html
     * - http://man7.org/linux/man-pages/man3/getopt.3.html
    */
    // Set this so that `getopt` doesn't write error messages.
    opterr = 0;
    int current_option;
    // As per the man page, calling `getopt` parses the arguments from argv,
    // as specified in the third param, `optstring`. When there are no option
    // characters left, `getopt` returns -1.
    while ((current_option = getopt(argc, argv, "hi:o:t:")) != -1) {
       switch (current_option) {
        case 'h':
            print_help_and_terminate(argv);
            break;
        case 'i':
            program_opts->input_file = malloc(sizeof(char) * strlen(optarg));
            strcpy(program_opts->input_file, optarg);
            break;
        case 'o':
            program_opts->output_file = malloc(sizeof(char) * strlen(optarg));
            strcpy(program_opts->output_file, optarg);
            break;
        case 't':
            program_opts->allowable_run_time = atol(optarg);
            break;
        // From the man page:
        // By default, getopt() prints an error message on standard error,
        // places the erroneous option character in optopt, and returns '?'
        // as the function result.
        // Note that `getopt` sets a global `optopt` with the option character.
        case '?':
            // If the error is for not having an arg with one of the params . . . 
            if (optopt == 'o' || optopt == 'i' || optopt == 't') {
                fprintf(stderr, "%s: Error: Argument `%c` requires a parameter\n", argv[0], optopt);
            // Unknown opt
            } else {
                fprintf(stderr, "%s: Error: Unkown option character -%c\n", argv[0], optopt);
            }
            // In either case, terminate.
            print_help_and_terminate(argv);
            break;
       }
    }

    if (argv[optind] != NULL) {
        fprintf(stderr, "%s: Unexpected non-option argument: %s\n", argv[0], argv[optind]);
        print_help_and_terminate(argv);
    }

    if (program_opts->input_file == NULL) {
        const char default_input_file_path[] = "input.dat";
        const unsigned int default_input_path_len = strlen(default_input_file_path);
        program_opts->input_file = (char*) malloc(sizeof(char) * default_input_path_len);
        strcpy(program_opts->input_file, default_input_file_path);
    }

    if (program_opts->output_file == NULL) {
        const char default_output_file_path[] = "output.dat";
        const unsigned int default_output_path_len = strlen(default_output_file_path);
        program_opts->output_file = (char*) malloc(sizeof(char) * default_output_path_len);
        strcpy(program_opts->output_file, default_output_file_path);
    }
}

