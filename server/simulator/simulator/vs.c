#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cjson/cJSON.h>
#include "simulator.h"
#include "node.h"
#include "vs.h"
#include <math.h>

#define PI 3.1415926535f
#define BUFF_SIZE 258
#define ROTATIONS_PER_SECOND 0.25
#define max(x1,x2) ((x1) > (x2) ? (x1) : (x2))
#define min(x1,x2) ((x1) < (x2) ? (x1) : (x2))

cJSON *root;
char buffer [BUFF_SIZE];
unsigned short buffer_pos = 0;

void init(char *json) {

}

float readDistanceSensor(short index) {
    return 0.0;
}

int check_intersection(struct line l1, struct line l2) {
    //get the slopes of the two lines
    float m1 = (l1.p1.y - l1.p2.y) / (l1.p1.x - l1.p2.x);
    float m2 = (l2.p1.y - l2.p2.y) / (l2.p1.x - l2.p2.x);
    //y=mx+b, calculate the b values for both lines
    float b1 = l1.p1.y - m1*l1.p1.x;
    float b2 = l2.p1.y - m2*l2.p1.x;

    if (m1 == m2) {
        return 0;
    }

    float intersection_x = (b2-b1)/(m1-m2);
    float left_bound = max(min(l2.p1.x, l2.p2.x), min(l1.p1.x, l1.p2.x));
    float right_bound = min(max(l2.p1.x, l2.p2.x), max(l1.p1.x, l1.p2.x)); 
    if (intersection_x < right_bound && intersection_x > left_bound) {
        return 1;
    } else {
        return 0;
    }
}

int check_for_collisions(struct arena *arena) {
    int i, j, k;
    // struct coordinates a,b,c,d represent the four corners of the OSV
    double cos_theta = cos(arena->osv.location.theta);
    double sin_theta = sin(arena->osv.location.theta);

    struct coordinate coordinateFront;
    coordinateFront.x = arena->osv.location.x + arena->osv.width / 2 * cos_theta;
    coordinateFront.y = arena->osv.location.y + arena->osv.width / 2 * sin_theta;

    struct coordinate a;
    a.x = coordinateFront.x - arena->osv.height / 2 * sin_theta;
    a.y = coordinateFront.y + arena->osv.height / 2 * cos_theta;

    struct coordinate b;
    b.x = coordinateFront.x + arena->osv.height / 2 * sin_theta;
    b.y = coordinateFront.y - arena->osv.height / 2 * cos_theta;

    struct coordinate coordinateBack;
    coordinateBack.x = arena->osv.location.x - arena->osv.width / 2 * cos_theta;
    coordinateBack.y = arena->osv.location.y - arena->osv.width / 2 * sin_theta;

    struct coordinate c;
    c.x = coordinateBack.x - arena->osv.height / 2 * sin_theta;
    c.y = coordinateBack.y + arena->osv.height / 2 * cos_theta;

    struct coordinate d;
    d.x = coordinateBack.x + arena->osv.height / 2 * sin_theta;
    d.y = coordinateBack.y - arena->osv.height / 2 * cos_theta;

    struct line front_osv = {a.x, a.y, b.x, b.y};
    struct line left_osv = {a.x, a.y, c.x, c.y};
    struct line back_osv = {c.x, c.y, d.x, d.y};
    struct line right_osv = {b.x, b.y, d.x, d.y};

    struct line osv_sides [4] = {front_osv, left_osv, back_osv, right_osv};

    for(i=0; i < arena->num_obstacles; i++) {
        // for each of the obstacles
        struct line right = {arena->obstacles[i].location.x + arena->obstacles[i].height, arena->obstacles[i].location.y, arena->obstacles[i].location.x + arena->obstacles[i].height, arena->obstacles[i].location.y - arena->obstacles[i].width};
        struct line bottom = {arena->obstacles[i].location.x, arena->obstacles[i].location.y - arena->obstacles[i].width, arena->obstacles[i].location.x + arena->obstacles[i].height, arena->obstacles[i].location.y - arena->obstacles[i].width};
        struct line left = {arena->obstacles[i].location.x, arena->obstacles[i].location.y - arena->obstacles[i].width, arena->obstacles[i].location.x, arena->obstacles[i].location.y};
        struct line top = {arena->obstacles[i].location.x, arena->obstacles[i].location.y, arena->obstacles[i].location.x + arena->obstacles[i].height, arena->obstacles[i].location.y};
        struct line obstacle_sides [4] = {right, bottom, left, top};

        for(j=0; j<sizeof(obstacle_sides); j++) {
            for(k=0; k<sizeof(osv_sides); k++) {
                if(check_intersection(osv_sides[k], obstacle_sides[j])) {
                    return 1;
                }
            }

        }
    }

    // now we want to define the walls:
    struct line right = {4.0, 0.0, 4.0, 2.0};
    struct line bottom = {0.0, 0.0, 4.0, 0.0};
    struct line left = {0.0, 0.0, 0.0, 2.0};
    struct line top = {0.0, 2.0, 4.0, 2.0};
    struct line walls [4] = {right, bottom, left, top};

    //need to check right and left sides of OSV in case OSV is perpendicular to wall
    for(i=0; i<sizeof(walls); i++) {
        for(j=0; j<sizeof(osv_sides); j++) {
            if(check_intersection(osv_sides[j], walls[i])) {
                return 1;
            }
        }
    }
    return 0;
}

void update_osv(struct arena *arena) {

    struct coordinate prev_location;
    prev_location.x = arena->osv.location.x;
    prev_location.y = arena->osv.location.y;
    prev_location.theta = arena->osv.location.theta;

    double speed = (arena->osv.right_motor_pwm + arena->osv.left_motor_pwm) / (255.0 * 50.0);

    arena->osv.location.x = arena->osv.location.x + speed * cos(arena->osv.location.theta);
    arena->osv.location.y = arena->osv.location.y + speed * sin(arena->osv.location.theta);

    arena->osv.location.theta += 2 * PI * ROTATIONS_PER_SECOND / 50 * (arena->osv.right_motor_pwm - arena->osv.left_motor_pwm) / 255.0;

    if(check_for_collisions(arena)) {
        arena->osv.location.x = prev_location.x;
        arena->osv.location.y = prev_location.y;
        arena->osv.location.theta = prev_location.theta;
    }

}

struct node * process_command(struct node *in, struct process p, struct arena *arena) {
    char opcode;
    int i;
    unsigned char ack_code = '\x07';
    struct node *curr, *next;
    buffer_pos = 0;

    if (in == NULL || in->size == 0) {
        return in;
    }

    //read in all of the available data into the buffer
    while (curr != NULL && buffer_pos < BUFF_SIZE) {
        for(i=0; i<curr->size; i++) {
            buffer[i + buffer_pos] = (curr->data)[i];
        }
        buffer_pos += curr->size;
        curr = curr-> next;
    }

    //we are at the beginning of a message, check for opcode
    opcode = buffer[0];
    if (opcode == 0x00) {
        //Enes100.begin() message
        //receives: 1 byte opcode
        //returns: 3 floats
        write(p.output_fd, &(arena->destination.x), sizeof(float *));
        write(p.output_fd, &(arena->destination.y), sizeof(float *));
        write(p.output_fd, &(arena->destination.theta), sizeof(float *));
    } else if (opcode == 0x01) {
        //updateLocation() message
        //receives: 1 byte opcode
        //returns: 3 floats
        write(p.output_fd, &(arena->osv.location.x), sizeof(float *));
        write(p.output_fd, &(arena->osv.location.x), sizeof(float *));
        write(p.output_fd, &(arena->osv.location.x), sizeof(float *));  
    } else if (opcode == 0x02) {
        //println() message
        //receives: 1 byte opcode, 1 byte length, length number of characters
        //returns: 1 byte ack
        if (in->size == 1 || in->size < ((in->data)[1] - 2)) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
        }
    } else if (opcode == 0x03) {
        //Tank.setLeftMotorPWM()
        //receives: 1 byte opcode, 2 byte pwm value
        //returns: 1 byte ack
        if (in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
            arena->osv.left_motor_pwm = buffer[1] << 8 + buffer[2];
        }
    } else if (opcode == 0x04) {
        //Tank.setRightMotorPWM()
        //receives: 1 byte opcode, 2 byte pwm value
        //returns: 1 byte ack
        if (in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
            arena->osv.right_motor_pwm = buffer[1] << 8 + buffer[2];
        }
    } else if (opcode == 0x05) {
        //Tank.turnOffMotors()
        //receives: 1 byte opcode
        //returns: 1 byte ack
        arena->osv.left_motor_pwm = 0;
        arena->osv.right_motor_pwm = 0;
        write(p.output_fd, &ack_code, sizeof(int *));
    } else if (opcode == 0x06) {
        //Tank.readDistanceSensors()
        //receives: 1 byte opcode, 1 byte index
        //returns: 4 byte float
        if (in->size < 2) {
            return in;
        } else {
            float distVal = readDistanceSensor((short)buffer[1]);
            write(p.output_fd, &distVal, sizeof(float *));
        }
    } else {
        //error("Invalid opcode\n");
    }

    //free all of the data
    while (curr != NULL) {
        next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }
    return NULL;    
}

struct node * frame(struct node *in, struct process p, struct arena *arena) {
    // in is a linked list with the current command in parts
    // the entire list will always contain a total of one commands or less
    struct node * ret_node = process_command(in, p, arena);

    static int frame_no = 0;
    cJSON *root = cJSON_CreateObject();
    cJSON *osv = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "frame_no", frame_no);
    cJSON_AddNumberToObject(osv, "x", arena->osv.location.x);
    cJSON_AddNumberToObject(osv, "y", arena->osv.location.y);
    cJSON_AddNumberToObject(osv, "theta", arena->osv.location.theta);
    cJSON_AddItemToObject(root, "osv", osv);
    frame_no++;

    printf("%s,", cJSON_Print(root));
    cJSON_Delete(root); 

    return in;
}
