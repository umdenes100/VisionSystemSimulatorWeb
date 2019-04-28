#ifndef VS_H
#define VS_H

#include "simulator.h"
#include "node.h"

void init(char *json);
struct node * frame(struct node *in, struct process p);

#endif