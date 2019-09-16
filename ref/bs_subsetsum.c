#include <stdio.h>
#include <stdlib.h>


void display_subset(int[], int);


void find_subset(int*, int, int, long);


int subset_sum(int*, int*, int, int, int, int, int);


int main() {
    int weights[] = {10, 7, 5, 18, 12, 20, 15};
    int size = 7;
    find_subset(weights, size, 35, 0l);
    return EXIT_SUCCESS;
}


void find_subset(int* set, int size, int sum, long pid) {
    int* subset = (int*) malloc(size * sizeof(int));

    if (subset == NULL) {
        perror("Unable to allocate memory");
        return;
    }

    if (!subset_sum(set, subset, size, 0, 0, 0, sum)) {
        printf("%ld No subset with sum of %d\n", pid, sum);
    }

    free(subset);
}


int subset_sum(int* set, int* subset, int n, int subset_size, int total, int node_count, int sum) {
    if (total == sum) {
        display_subset(subset, subset_size);
        return 1;
    } else {
        // Iterate over the breadth of the option tree
        int i;
        for (int i = node_count; i < n; ++i) {
            subset[subset_size] = set[i];
            if (subset_sum(set, subset, n, subset_size + 1, total + set[i], i + 1, sum) == 1) {
                return 1;
            }
        }
        return 0;
    }
}


void display_subset(int subset[], int length) {
    int i;
    for (i = 0; i < length; ++i) {
        printf("%d ", subset[i]);
    }
    printf("\n");
}