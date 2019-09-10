#include <time.h>
#include <stdlib.h>
#include <stdio.h>


void display_primes_to(unsigned int);


int is_prime(unsigned int);


int main(void) {
    /* We are going to compute the number of seconds between
    ** two computations. To make this interesting, I'll compute
    ** some primes. Would be cool to do like uhhh. Mersenne 
    ** primes, but I need to be pragmattic here.
    */
    time_t start_time, stop_time;

    start_time = time(NULL);
    display_primes_to(1000000);
    stop_time = time(NULL);

    printf("[+] Function start at %ld seconds\n", start_time);
    printf("[+] Function stop at %ld seconds\n", stop_time);
    printf("[+] Function took %f seconds\n", difftime(stop_time, start_time));

    return EXIT_SUCCESS;
}

void display_primes_to(unsigned int max) {
    /* This is not a great algorithm for this. 
    ** The sieve of eratosthenes would be better.
    ** Still not best. 
    */
    unsigned int test_number;
    unsigned int i;
    for (test_number = 2; test_number <= max; ++test_number) {
        if (is_prime(test_number)) {
            printf("[+] Prime: %d\n", test_number);
        }
    }
}


int is_prime(unsigned int to_test) {
    int i;
    if (i == 0 || i == 1) {
        return 0;
    } else if (i == 2) {
        return 1;
    }
    // We don't actually need to go all the way to `to_test`. 
    // There's a faster way to do this. But I'm not going to.
    // Instead, I'm trying to use up time.
    for (i = 2; i < to_test; ++i) {
        if (to_test % i == 0) {
            return 0;
        }
    }
    return 1;
}
