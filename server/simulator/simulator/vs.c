#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/time.h>
#include <cjson/cJSON.h>
#include "simulator.h"
#include "node.h"

#define ack_code 0x07

cJSON *root;
char [256] buffer;
unsigned short buffer_pos = 0;

void init(char *json) {

}

struct node * frame(struct node *in, struct process p) {
    char opcode;
    int i;
    if (in->size == 0 || in == NULL) {
        return in;
    }
    if (buffer_pos > 0) {
        if (in->next == NULL) {
            error("Expected node, got NULL\n");
            return in;
        }
        
    }
    //we are at the beginning of a message, check for opcode
    buffer_pos ++;
    opcode = (in->data)[0]
    if (opcode == 0x00) {
        //Enes100.begin() message
        //receives: 1 byte opcode
        //returns: 3 floats
        write(p.output_fd, &(arena->destination->x), sizeof(float *));
        write(p.output_fd, &(arena->destination->y), sizeof(float *));
        write(p.output_fd, &(arena->destination->theta), sizeof(float *));
        return NULL;
    } else if (opcode == 0x01) {
        //updateLocation() message
        //receives: 1 byte opcode
        //returns: 3 floats
        write(p.output_fd, &(arena->osv->x), sizeof(float *));
        write(p.output_fd, &(arena->osv->x), sizeof(float *));
        write(p.output_fd, &(arena->osv->x), sizeof(float *));  
        return NULL;          
    } else if (opcode == 0x02) {
        //println() message
        //receives: 1 byte opcode, 1 byte length, length number of characters
        //returns: 1 byte ack
        if (in->size == 1 || in->size < ((in->data)[1] - 2)) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
            return NULL;
        }
    } else if (opcode == 0x03) {
        //Tank.setLeftMotorPWM()
        //receives: 1 byte opcode, 2 byte pwm value
        //returns: 1 byte ack
        if (in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
            return NULL;
        }
    } else if (opcode == 0x04) {
        //Tank.setRightMotorPWM()
        //receives: 1 byte opcode, 2 byte pwm value
        //returns: 1 byte ack
        if (in->size < 3) {
            return in;
        } else {
            write(p.output_fd, &ack_code, sizeof(int *));
            return NULL;
        }
    } else if (opcode == 0x05) {
        //Tank.turnOffMotors()
        //receives: 1 byte opcode
        //returns: 1 byte ack
        write(p.output_fd, &ack_code, sizeof(int *));
        return NULL;
    } else if (opcode == 0x06) {
        //Tank.readDistanceSensors()
        //receives: 1 byte opcode, 1 byte index
        //returns: 4 byte float
        if (in->size < 2) {
            return in;
        } else {
            write(p.output_fd, readDistanceSensor((short)((in->data)[1])), sizeof(float *));
            return NULL;
        }
    } else {
        //opcode error
    }
    buffer_pos = 0;

    /*
    for (i=0; i < in->size; i++) {
        buffer[i+buffer_pos] = (in->data)[i];
    }
    */

    // in is a linked list with the current command in parts
    // the entire list will always contain a total of one commands or less

    /*
     * #include <stdio.h>
     * #include <stdlib.h>
     *
     * void setup() {
     *     printf("1");
     *     fflush(stdout);
     * }
     * 
     * void loop() {
     *     char buff[14];
     *     if(fgets(buff, 14, stdin) != NULL) {
     *         printf("got '%s', sending now...", buff);
     *         fflush(stdout);
     *     }
     * }
     * 
    */

    if(in != NULL) {
        printf("recv'd: '%s'\n", in->data);
        write(p.output_fd, "hello process", strlen("hello process"));
        in = in->next;
    }

    return in;
}

float readDistanceSensor(int index) {
    return 0.0;
}
