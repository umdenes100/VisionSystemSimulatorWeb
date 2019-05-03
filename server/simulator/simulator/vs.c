#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cjson/cJSON.h>
#include <math.h>

#include "simulator.h"
#include "node.h"
#include "vs.h"

#define SENSOR_RANGE 1.0f
#define PI 3.1415926535f
#define BUFF_SIZE 258
#define ROTATIONS_PER_SECOND 0.25f
#define max(x1,x2) ((x1) > (x2) ? (x1) : (x2))
#define min(x1,x2) ((x1) < (x2) ? (x1) : (x2))

char buffer [BUFF_SIZE];
unsigned short buffer_pos = 0;

float distance(struct coordinate a, struct coordinate b) {
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2));
}

float read_distance_sensor(struct arena arena, short index) {
    int i, j;

    if(!arena.osv.distance_sensors[index]) {
        return -1.0;
    }

    // we have to get the slope of the front side of the osv first
    float cos_theta = cos(arena.osv.location.theta);
    float sin_theta = sin(arena.osv.location.theta);

    struct coordinate midPointFront;
    midPointFront.x = arena.osv.location.x + arena.osv.height / 2 * cos_theta;
    midPointFront.y = arena.osv.location.y + arena.osv.height / 2 * sin_theta;

    struct coordinate a;
    a.x = midPointFront.x - arena.osv.width / 2 * sin_theta;
    a.y = midPointFront.y + arena.osv.width / 2 * cos_theta;

    struct coordinate b;
    b.x = midPointFront.x + arena.osv.width / 2 * sin_theta;
    b.y = midPointFront.y - arena.osv.width / 2 * cos_theta;

    struct coordinate midPointBack;
    midPointBack.x = arena.osv.location.x - arena.osv.height / 2 * cos_theta;
    midPointBack.y = arena.osv.location.y - arena.osv.height / 2 * sin_theta;

    struct coordinate c;
    c.x = midPointBack.x - arena.osv.width / 2 * sin_theta;
    c.y = midPointBack.y + arena.osv.width / 2 * cos_theta;

    struct coordinate d;
    d.x = midPointBack.x + arena.osv.width / 2 * sin_theta;
    d.y = midPointBack.y - arena.osv.width / 2 * cos_theta;

    struct coordinate midPointLeft;
    midPointLeft.x = (a.x + c.x) / 2;
    midPointLeft.y = (a.y + c.y) / 2;

    struct coordinate midPointRight;
    midPointRight.x = (b.x + d.x) / 2;
    midPointRight.y = (b.y + d.y) / 2;

    struct coordinate sensor_locations[12] = {a, midPointFront, b, b, midPointRight, d, d, midPointBack, c, c, midPointLeft, a};

    int sideIndex = index / 3;
    float orientation = arena.osv.location.theta - sideIndex * PI / 2;

    struct coordinate endPoint;
    endPoint.x = sensor_locations[index].x + SENSOR_RANGE * cos(orientation);
    endPoint.y = sensor_locations[index].y + SENSOR_RANGE * sin(orientation);

    struct line sensor_trace;
    sensor_trace.p1.x = sensor_locations[index].x;
    sensor_trace.p1.y = sensor_locations[index].y;
    sensor_trace.p2.x = endPoint.x;
    sensor_trace.p2.y = endPoint.y;

    float minimum_distance = 1.0;

    for(i = 0; i < arena.num_obstacles; i++) {
        // for each of the arena.obstacles[i]s
        struct line right;
        struct coordinate r1, r2;
        r1.x = arena.obstacles[i].location.x + arena.obstacles[i].width;
        r1.y = arena.obstacles[i].location.y;
        r2.x = arena.obstacles[i].location.x + arena.obstacles[i].width;
        r2.y = arena.obstacles[i].location.y - arena.obstacles[i].height;
        right.p1 = r1; right.p2 = r2;

        struct line bottom;
        struct coordinate b1, b2;
        b1.x = arena.obstacles[i].location.x;
        b1.y = arena.obstacles[i].location.y - arena.obstacles[i].height;
        b2.x = arena.obstacles[i].location.x + arena.obstacles[i].width;
        b2.y = arena.obstacles[i].location.y - arena.obstacles[i].height;
        bottom.p1 = b1; bottom.p2 = b2;

        struct line left;
        struct coordinate l1, l2;
        l1.x = arena.obstacles[i].location.x;
        l1.y = arena.obstacles[i].location.y - arena.obstacles[i].height;
        l2.x = arena.obstacles[i].location.x;
        l2.y = arena.obstacles[i].location.y;
        left.p1 = l1; left.p2 = l2;

        struct line top;
        struct coordinate t1, t2;
        t1.x = arena.obstacles[i].location.x;
        t1.y = arena.obstacles[i].location.y;
        t2.x = arena.obstacles[i].location.x + arena.obstacles[i].width;
        t2.y = arena.obstacles[i].location.y;
        top.p1 = t1; top.p2 = t2;
        
        struct line obstacle_sides[4] = {right, bottom, left, top};

        for(j = 0; j < 4; j++) {
        	struct coordinate *intersection_point = get_intersection(obstacle_sides[j], sensor_trace);
            if(intersection_point != NULL) {
                //todo: need to find point on obstacle that is closest to the sensor location, and then find the distance
                minimum_distance = min(minimum_distance, distance(sensor_locations[index], *intersection_point));
            }
        }
    }

    return minimum_distance;
}

struct line init_line(float x1, float y1, float x2, float y2) {
    struct line temp;
    struct coordinate hold;
    
    hold.x = x1;
    hold.y = y1;
    temp.p1 = hold;
    
    hold.x = x2;
    hold.y = y2;
    temp.p2 = hold;

    return temp;
}

int on_segment(struct coordinate p, struct coordinate q, struct coordinate r) { 
    if(q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) && q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y)) {
        return 1; 
    }
  
    return 0; 
} 
  
// To find orientation of ordered triplet (p, q, r). 
// The function returns following values 
// 0 --> p, q and r are colinear 
// 1 --> Clockwise 
// 2 --> Counterclockwise 
int orientation(struct coordinate p, struct coordinate q, struct coordinate r) { 
    // See https://www.geeksforgeeks.org/orientation-3-ordered-points/ 
    // for details of below formula. 
    int val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y); 
    if (val == 0) return 0;     // colinear 
    return (val > 0) ? 1 : 2;   // clock or counterclock wise 
} 
  
// The main function that returns true if line segment 'p1q1' 
// and 'p2q2' intersect. 
int do_intersect(struct coordinate p1, struct coordinate q1, struct coordinate p2, struct coordinate q2) { 
    // Find the four orientations needed for general and 
    // special cases 
    int o1 = orientation(p1, q1, p2); 
    int o2 = orientation(p1, q1, q2); 
    int o3 = orientation(p2, q2, p1); 
    int o4 = orientation(p2, q2, q1); 
  
    // General case 
    if (o1 != o2 && o3 != o4) {
        return 1; 
    }

    // Special Cases 
    // p1, q1 and p2 are colinear and p2 lies on segment p1q1 
    if (o1 == 0 && on_segment(p1, p2, q1)) return 1; 
  
    // p1, q1 and q2 are colinear and q2 lies on segment p1q1 
    if (o2 == 0 && on_segment(p1, q2, q1)) return 1; 
  
    // p2, q2 and p1 are colinear and p1 lies on segment p2q2 
    if (o3 == 0 && on_segment(p2, p1, q2)) return 1; 
  
    // p2, q2 and q1 are colinear and q1 lies on segment p2q2 
    if (o4 == 0 && on_segment(p2, q1, q2)) return 1; 
  
    return 0;   // Doesn't fall in any of the above cases 
} 

int check_intersection(struct line l1, struct line l2) {
    do_intersect(l1.p1, l1.p2, l2.p1, l2.p2);
}

int check_for_collisions(struct arena *arena) {
    int i, j, k;
    // struct coordinates a,b,c,d represent the four corners of the OSV
    double cos_theta = cos(arena->osv.location.theta);
    double sin_theta = sin(arena->osv.location.theta);

    struct coordinate coordinate_front;
    coordinate_front.x = arena->osv.location.x + arena->osv.width / 2 * cos_theta;
    coordinate_front.y = arena->osv.location.y + arena->osv.width / 2 * sin_theta;

    struct coordinate a;
    a.x = coordinate_front.x - arena->osv.height / 2 * sin_theta;
    a.y = coordinate_front.y + arena->osv.height / 2 * cos_theta;

    struct coordinate b;
    b.x = coordinate_front.x + arena->osv.height / 2 * sin_theta;
    b.y = coordinate_front.y - arena->osv.height / 2 * cos_theta;

    struct coordinate coordinate_back;
    coordinate_back.x = arena->osv.location.x - arena->osv.width / 2 * cos_theta;
    coordinate_back.y = arena->osv.location.y - arena->osv.width / 2 * sin_theta;

    struct coordinate c;
    c.x = coordinate_back.x - arena->osv.height / 2 * sin_theta;
    c.y = coordinate_back.y + arena->osv.height / 2 * cos_theta;

    struct coordinate d;
    d.x = coordinate_back.x + arena->osv.height / 2 * sin_theta;
    d.y = coordinate_back.y - arena->osv.height / 2 * cos_theta;

    struct line front_osv;
    front_osv.p1 = a;
    front_osv.p2 = b;
    
    struct line left_osv;
    left_osv.p1 = a;
    left_osv.p2 = c;

    struct line back_osv;
    back_osv.p1 = c;
    back_osv.p2 = d;

    struct line right_osv;
    right_osv.p1 = b;
    right_osv.p2 = d;

    struct line osv_sides[4] = {front_osv, left_osv, back_osv, right_osv};

    for(i = 0; i < arena->num_obstacles; i++) {
        // for each of the obstacles
        struct line right;
        struct coordinate r1, r2;
        r1.x = arena->obstacles[i].location.x + arena->obstacles[i].width;
        r1.y = arena->obstacles[i].location.y;
        r2.x = arena->obstacles[i].location.x + arena->obstacles[i].width;
        r2.y = arena->obstacles[i].location.y - arena->obstacles[i].height;
        right.p1 = r1; right.p2 = r2;

        struct line bottom;
        struct coordinate b1, b2;
        b1.x = arena->obstacles[i].location.x;
        b1.y = arena->obstacles[i].location.y - arena->obstacles[i].height;
        b2.x = arena->obstacles[i].location.x + arena->obstacles[i].width;
        b2.y = arena->obstacles[i].location.y - arena->obstacles[i].height;
        bottom.p1 = b1; bottom.p2 = b2;

        struct line left;
        struct coordinate l1, l2;
        l1.x = arena->obstacles[i].location.x;
        l1.y = arena->obstacles[i].location.y - arena->obstacles[i].height;
        l2.x = arena->obstacles[i].location.x;
        l2.y = arena->obstacles[i].location.y;
        left.p1 = l1; left.p2 = l2;

        struct line top;
        struct coordinate t1, t2;
        t1.x = arena->obstacles[i].location.x;
        t1.y = arena->obstacles[i].location.y;
        t2.x = arena->obstacles[i].location.x + arena->obstacles[i].width;
        t2.y = arena->obstacles[i].location.y;
        top.p1 = t1; top.p2 = t2;
        
        struct line obstacle_sides[4] = {right, bottom, left, top};

        for(j = 0; j < 4; j++) {
            for(k = 0; k < 4; k++) {
                if(check_intersection(osv_sides[k], obstacle_sides[j])) {         
                    return 1;
                }
            }
        }
    }

    // now we want to define the walls:
    struct line right;
    struct coordinate r1, r2;
    r1.x = 4.0;
    r1.y = 0.0;
    r2.x = 4.0;
    r2.y = 2.0;
    right.p1 = r1; right.p2 = r2;
    
    struct line bottom;
    struct coordinate b1, b2;
    b1.x = 0.0;
    b1.y = 0.0;
    b2.x = 4.0;
    b2.y = 0.0;
    bottom.p1 = b1; bottom.p2 = b2;

    struct line left;
    struct coordinate l1, l2;
    l1.x = 0.0;
    l1.y = 0.0;
    l2.x = 0.0;
    l2.y = 2.0;
    bottom.p1 = l1; bottom.p2 = l2;

    struct line top;
    struct coordinate t1, t2;
    t1.x = 0.0;
    t1.y = 2.0;
    t2.x = 4.0;
    t2.y = 2.0;
    top.p1 = t1; top.p2 = t2;

    struct line walls[4] = {right, bottom, left, top};

    // need to check right and left sides of OSV in case OSV is perpendicular to wall
    for(i = 0; i < 4; i++) {
        for(j = 0; j < 4; j++) {
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

    if(in == NULL || in->size == 0) {
        return in;
    }

    curr = in;
    // read in all of the available data into the buffer
    while(curr != NULL && (buffer_pos < BUFF_SIZE)) {
        for(i = 0; i < curr->size; i++) {
            buffer[i + buffer_pos] = (curr->data)[i];
        }

        buffer_pos += curr->size;
        curr = curr->next;
    }

    // we are at the beginning of a message, check for opcode
    opcode = buffer[0];
    if(opcode == 0x00) {
        // Enes100.begin() message
        // receives: 1 byte opcode
        // returns: 3 floats
        write(p.output_fd, &(arena->destination.x), sizeof(float));
        write(p.output_fd, &(arena->destination.y), sizeof(float));
        write(p.output_fd, &(arena->destination.theta), sizeof(float));
    } else if(opcode == 0x01) {
        // updateLocation() message
        // receives: 1 byte opcode
        // returns: 3 floats
        write(p.output_fd, &(arena->osv.location.x), sizeof(float));
        write(p.output_fd, &(arena->osv.location.y), sizeof(float));
        write(p.output_fd, &(arena->osv.location.theta), sizeof(float));  
    } else if(opcode == 0x02) {
        // println() message
        // receives: 1 byte opcode, 1 byte length, length number of characters
        // returns: 1 byte ack
        if(in->size == 1 || buffer[1] < buffer_pos - 2) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(unsigned char));
        }
    } else if(opcode == 0x03) {
        // Tank.setLeftMotorPWM()
        // receives: 1 byte opcode, 2 byte pwm value
        // returns: 1 byte ack
        if(in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(unsigned char));
            arena->osv.left_motor_pwm = (buffer[1] << 8) + buffer[2];
        }
    } else if(opcode == 0x04) {
        // Tank.setRightMotorPWM()
        // receives: 1 byte opcode, 2 byte pwm value
        // returns: 1 byte ack
        if(in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(unsigned char));
            arena->osv.right_motor_pwm = (buffer[1] << 8) + buffer[2];
        }
    } else if(opcode == 0x05) {
        // Tank.turnOffMotors()
        // receives: 1 byte opcode
        // returns: 1 byte ack
        arena->osv.left_motor_pwm = 0;
        arena->osv.right_motor_pwm = 0;
        write(p.output_fd, &ack_code, sizeof(unsigned char));
    } else if(opcode == 0x06) {
        // Tank.readDistanceSensors()
        // receives: 1 byte opcode, 1 byte index
        // returns: 4 byte float
        if(buffer_pos < 2) {
            return in;
        } else {
            float distVal = read_distance_sensor(*arena, (short)buffer[1]);
            write(p.output_fd, &distVal, sizeof(float));
        }
    } else {
        // error("Invalid opcode\n");
    }

    // free all of the data
    while (curr != NULL) {
        next = curr->next;
        free(curr->data);
        free(curr);
        curr = next;
    }

    return NULL;    
}

struct node * frame(struct node *in, struct process p, struct arena *arena, int frame_no) {
    update_osv(arena);
    struct node * ret_node = process_command(in, p, arena);

    cJSON *root = cJSON_CreateObject();
    cJSON *osv = cJSON_CreateObject();
    cJSON_AddNumberToObject(root, "frame_no", frame_no);
    cJSON_AddNumberToObject(osv, "x", arena->osv.location.x);
    cJSON_AddNumberToObject(osv, "y", arena->osv.location.y);
    cJSON_AddNumberToObject(osv, "theta", arena->osv.location.theta);
    cJSON_AddItemToObject(root, "osv", osv);

    printf("%s,", cJSON_Print(root));
    cJSON_Delete(root);

    return ret_node;
}
