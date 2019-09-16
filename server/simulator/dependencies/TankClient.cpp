#include "TankClient.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

TankClient::TankClient() {

};

void TankClient::begin(){
    // Do what we want.
    this->init = true;
};

void TankClient::setLeftMotorPWM(int ln, short pwm) {
    // Do what we want.
    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }

    if (this->init) {
        fputc('\x03', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(pwm), stdout);
        fputc((char)(pwm >> 8), stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');  // Waits to read a byte equaling
                                         // the hexadecimal '0x08'.
    }
};

void TankClient::setRightMotorPWM(int pwm) {
    // Do what we want.
    if (pwm > 255) {
        pwm = 255;
    } else if (pwm < -255) {
        pwm = -255;
    }

    if (this->init) {
        fputc('\x04', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(pwm), stdout);
        fputc((char)(pwm >> 8), stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');  // Waits to read a byte equaling
                                         // the hexadecimal '0x08'.
    }

};

void TankClient::turnOffMotors(){
    // Do what we want.
    if (this->init) {
        fputc('\x05', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fflush(stdout);
        while (fgetc(stdin) != '\x08');  // Waits to read a byte equaling
                                         // the hexadecimal '0x08'.

    }
};
float TankClient::readDistanceSensor(int ln, int id) {
    // Do what we want.
    if (id > 11) {  // There are only 12 possible distance sensor placements
                    // on the simulator.
        return -1.0;  // Return an error value.
    }

    if (this->init) {
        fputc('\x06', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(id), stdout);
        fflush(stdout);

        char buff[4];
        buff[0] = fgetc(stdin);
        buff[1] = fgetc(stdin);
        buff[2] = fgetc(stdin);
        buff[3] = fgetc(stdin);

        return *(float *)buff;
    } else {  // This object was never initialized.
        return -1.0;
    }
}
