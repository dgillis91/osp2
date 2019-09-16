/* Dynamic programming implementation of
** https://www.geeksforgeeks.org/subset-sum-problem-dp-25/
** Note: I need to brush up on pointers. There are several ways
** to allocate a tensor of order two. See this article for 
** details:
** https://www.geeksforgeeks.org/dynamically-allocate-2d-array-c/
** This implementation uses #3, because it does what I need, and
** I beleive it's easiest to read.
*/
#include <stdlib.h>
#include <stdio.h>

int is_subset_sum(int set[], int set_length, int sumto) {
    int row_count = (set_length + 1);
    int col_count = (sumto + 1);
    
    int** subset = (int**) malloc(row_count * sizeof(int*));

    int current_row, current_col;
    for (current_row = 0; current_row < row_count; ++current_row) {
        subset[current_row] = (int*) malloc(col_count * sizeof(int));
    }

    // For the sum of 0, we can take the empty set.
    for (current_row = 0; current_row < row_count; ++current_row) {
        subset[current_row][0] = 1;
    }

    // Sum not 0, set empty
    for (current_col = 1; current_col < col_count; ++current_col) {
        subset[0][current_col] = 0;
    }

    for (current_row = 1; current_row < row_count; ++current_row) {
        for (current_col = 1; current_col < col_count; ++current_col) {
            if (current_col < set[current_row - 1]) {
                subset[current_row][current_col] = subset[current_row - 1][current_col];
            }
            if (current_col >= set[current_row - 1]) {
                subset[current_row][current_col] = subset[current_row - 1][current_col] || 
                                                   subset[current_row - 1][current_col - set[current_row - 1]];
            }
        }
    }

    printf("******MATRIX******\n");
    int i, j;
    for (i = 0; i < row_count; ++i) {
        for (j = 0; j < col_count; ++j) {
            printf("%d ", subset[i][j]);
        }
        printf("\n");
    }

    if (subset[set_length][sumto]) {
        printf("In if\n");
    }

    for (int i = 0; i < row_count; ++i) {
        free(subset[i]);
    }
    free(subset);

    return 1;
}

int main(void) {
    int set[] = {1, 2, 3, 4, 5};
    int sum = 10;
    int length = 5;

    is_subset_sum(set, length, sum);
}