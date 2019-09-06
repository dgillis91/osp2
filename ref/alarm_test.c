#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>


void alarm_handler(int signum);


int main(void) {
    if (signal(SIGALRM, alarm_handler) == SIG_ERR) {
        perror("Failed to register SIGALRM");
    }

    // Schedule SIGALRM to be raised in five seconds.
    alarm(5);
    // Wait for a signal.
    pause();
}


void alarm_handler(int signum) {
    printf("Alarm Raised\n");
}