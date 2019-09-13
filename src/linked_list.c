/* This list implementation has a few wonderful simplifications.
** Because we only need to:
**  - Add
**  - Display
** So, since we never have to delete from the list, we can
** assume that the last node always points to NULL. That is,
** when we add a node, we add it to the front. But, because
** nodes are allocated with a default, the next is originally
** NULL.
*/
#include "../include/linked_list.h"
#include <stdlib.h>
#include <stdio.h>


int malloc_default_node(list_node_t** node, int data) {
    (*node) = (list_node_t*) malloc(sizeof(list_node_t));
    if ((*node) == NULL) {
        return -1;
    }
    (*node)->next = NULL;
    (*node)->data = data;
    return 0;
}


int append(list_node_t** list, int data) {
    /* Append at the front because it's easy. */
    list_node_t* new_node;
    int malloc_stat;
    if ((malloc_stat = malloc_default_node(&new_node, data)) != 0) {
        return malloc_stat;
    }
    new_node->next = (*list);
    (*list) = new_node;
    return 1;
}


void display_list(list_node_t* list, char delimiter, FILE* io) {
    list_node_t* iterator = list;
    while (iterator != NULL) {
        fprintf(io, "%d%c", iterator->data, delimiter);
        iterator = iterator->next;
    }
}
