#include <stdio.h>
#include <string.h>
#include <stdlib.h>


int main(void) {
    char str[] = "10 -5 50 42 0 120";
    char* p = strtok(str, " ");

    while (p != NULL) {
        printf("%d\n", atoi(p));
        p = strtok(NULL, " ");
    }

    return EXIT_SUCCESS;
}