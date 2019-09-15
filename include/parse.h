#ifndef _PARSE_H
#define _PARSE_H

#include <time.h>


typedef struct program_options {
    char* input_file;
    char* output_file;
    unsigned int allowable_run_time;
} program_options_t;


program_options_t* malloc_default_program_options();


void free_program_options(program_options_t**);


void parse_options(int, char**, program_options_t*);


void print_help_and_terminate(char**);


#endif
