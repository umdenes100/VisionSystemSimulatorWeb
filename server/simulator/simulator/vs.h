#ifndef VS_H
#define VS_H

#include "simulator.h"
#include "node.h"


struct coordinate {
    float x;
    float y;
    float theta;
};

struct line {
    struct coordinate p1;
    struct coordinate p2;
};

struct obstacle {
    struct coordinate location;
    float width;
    float height;
};

struct osv {
    struct coordinate location;
    int *distance_sensors;
    float width, height;
    int left_motor_pwm, right_motor_pwm;
};

struct arena {
    struct obstacle *obstacles;
    int num_obstacles;
    struct coordinate destination;
    struct osv osv;
};

float readDistanceSensor(struct arena arena, short index);
struct coordinate* get_intersection(struct line l1, struct line l2);
struct node * frame(struct node *in, struct process p, struct arena *arena);
float distance(struct coordinate a, struct coordinate b);

#endif