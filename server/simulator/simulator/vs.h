#ifndef VS_H
#define VS_H

#include "simulator.h"
#include "node.h"

void init(char *json);
struct node * frame(struct node *in, struct process p);

struct coordinate {
    float x;
    float y;
    float theta;
};

struct obstacle {
    struct coordinate location;
    float width;
    float height;
};

struct osv {
    struct coordinate location;
    int *distance_sensors;
};

struct arena {
    struct obstacle *obstacles;
    struct osv osv;
};

#endif