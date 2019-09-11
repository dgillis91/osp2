#ifndef _LINKED_LIST_H
#define _LINKED_LIST_H
#include <stdio.h>

typedef struct list_node {
    struct list_node* next;
    int data;
} list_node_t;


int malloc_default_node(list_node_t**, int);


int append(list_node_t**, int);


void display_list(list_node_t*, char, FILE* restrict);

#endif