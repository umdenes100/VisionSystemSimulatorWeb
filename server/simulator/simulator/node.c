#include <stdio.h>
#include <stdlib.h>
#include "node.h"

struct node * new_node(char *curr_buff, int curr_size) {
    //How long does this list get? If it's long enough maybe implement a skip list
    struct node *n = (struct node *)malloc(1 * sizeof(struct node));    
    n->next = NULL;
    n->data = curr_buff;
    n->size = curr_size;
    return n;
}