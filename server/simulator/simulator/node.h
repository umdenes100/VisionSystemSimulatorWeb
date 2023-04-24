#ifndef NODE_H
#define NODE_H
//potentially make this a skip list??? should make it slightly faster
struct node {
    char *data;
    int size;
    struct node *next;
};

struct node * new_node(char *curr_buff, int curr_size);

#endif