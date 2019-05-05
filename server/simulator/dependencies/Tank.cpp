#include "Tank.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Tank::Tank() {

};

void Tank::begin(){
    // do what we want
    this->init = true;
};

void Tank::setLeftMotorPWM(short pwm) {
    // do what we want
    if(pwm > 255) {
        pwm = 255;
    } else if(pwm < -255) {
        pwm = -255;
    }

    if(this->init) {
        fputc('\x03', stdout);
        fputc((char)(pwm), stdout);
        fputc((char)(pwm >> 8), stdout);
        fflush(stdout);

        while(fgetc(stdin) != '\x08');
    }
};

void Tank::setRightMotorPWM(short pwm) {
    // do what we want
    if(pwm > 255) {
        pwm = 255;
    } else if(pwm < -255) {
        pwm = -255;
    }

    if(this->init) {
        fputc('\x04', stdout);
        fputc((char)(pwm), stdout);
        fputc((char)(pwm >> 8), stdout);
        fflush(stdout);

        while(fgetc(stdin) != '\x08');
    }
};

void Tank::turnOffMotors(){
    // do what we want
    if(this->init) {
        fputc('\x05', stdout);
        fflush(stdout);
        while(fgetc(stdin) != '\x08');
    }
};

float Tank::readDistanceSensor(int id) {
    // do what we want
    if(id > 11) {
        return -1.0;
    }

    if(this->init) {
        fputc('\x06', stdout);
        fputc((char)(id), stdout);
        fflush(stdout);
        
        char buff[4];
        fgets(buff, 4, stdin);
        float *val = (float *)buff;

        return *val;
    }
    
    return -1.0;
}