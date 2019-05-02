#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cjson/cJSON.h>
#include "simulator.h"
#include "node.h"
#include "vs.h"

#define BUFF_SIZE 258

cJSON *root;
char buffer [BUFF_SIZE];
unsigned short buffer_pos = 0;

void init(char *json) {

}

float readDistanceSensor(short index) {
    return 0.0;
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
