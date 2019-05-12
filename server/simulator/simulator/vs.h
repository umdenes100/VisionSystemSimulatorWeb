#ifndef VS_H
#define VS_H

#include "simulator.h"
#include "node.h"

#define PI 3.1415926535f
#define FE_FPS 60
#define SEC_TO_CROSS 12f
#define METERS_PER_FRAME (1/(FE_FPS * SEC_TO_CROSS / 4.0f))
#define SEC_TO_TURN 6.25f
#define RAD_PER_FRAME (1/(FE_FPS * SEC_TO_TURN / (2.0f * PI)))

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

float read_distance_sensor(struct arena arena, short index);
struct coordinate* get_intersection(struct line l1, struct line l2);
struct node * frame(struct node *in, struct process p, struct arena *arena);
float distance(struct coordinate a, struct coordinate b);

#endif