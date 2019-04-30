#ifndef NODE_H
#define NODE_H

struct node {
    char *data;
    int size;
    struct node *next;
};

struct node * new_node(char *curr_buff, int curr_size);

#endif