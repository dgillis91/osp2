#include "../include/tokenize.h"
#include <string.h>
#include <stdlib.h>


int token_count(const char* string, const char* delimiter) {
    /* Return the number of tokens in `string` as delimited by `delimiter`.
    ** We follow the format in "UNIX SYSTEM PROGRAMMING" in copying the
    ** string so that we can't modify it in `strtok`. Note that `strtok` DOES
    ** modify the string you pass in.
    */
    unsigned int string_length = strlen(string);
    char* temp;
    int numtokens;
    if ((temp = (char*) malloc(sizeof(char) * string_length + 1)) == NULL) {
        return -1;
    }
    strcpy(temp, string);
    if (strtok(temp, delimiter) != NULL) {
        for (numtokens = 1; strtok(NULL, delimiter) != NULL; ++numtokens) ;
    }
    free(temp);
    return numtokens;
}


int tokenize(const char* string, int* destination, char* delimiter) {
    char* temp;
    if ((temp = (char*) malloc(sizeof(int) * strlen(string) + 1)) == NULL) {
        return -1;
    }
    strcpy(temp, string);
    int i = 0;
    char* p = strtok(temp, delimiter);
    while (p != NULL) {
        destination[i++] = atoi(p);
        p = strtok(NULL, delimiter);
    }
    return 0;
}