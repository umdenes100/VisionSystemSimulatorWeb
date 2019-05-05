#include "VisionSystemClient.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Coordinate::Coordinate() {
    init(0, 0, 0);
}

Coordinate::Coordinate(double x, double y) {
    init(x, y, 0);
}

Coordinate::Coordinate(double x, double y, double theta) {
    init(x, y, theta);
}

void Coordinate::init(double x, double y, double theta) {
    this->x = x;
    this->y = y;
    this->theta = theta;
}

bool VisionSystemClient::ping() {
    // do nothing
}

bool VisionSystemClient::mission(int message) {
    // do nothing
}

bool VisionSystemClient::mission(double message) {
    // do nothing
}

bool VisionSystemClient::mission(Coordinate &message) {
    // do nothing
}

bool VisionSystemClient::begin(const char *teamName, int teamType, int markerId, int rxPin, int txPin) {
    // do what we want
    this->init = true;
    fputc('\x00', stdout);
    fflush(stdout);

    char x[5];
    fgets(x, 5, stdin);
    char y[5];
    fgets(y, 5, stdin);
    char theta[5];
    fgets(theta, 5, stdin);
    
    float x_f = *(float *)x;
    float y_f = *(float *)y;
    float theta_f = *(float *)theta;

    this->destination = Coordinate(x_f, y_f, theta_f);
    return true;
}

bool VisionSystemClient::updateLocation() {
    // do what we want
    if(this->init == true) {
        fputc('\x01', stdout);
        fflush(stdout);

        char x[5];
        fgets(x, 5, stdin);
        char y[5];
        fgets(y, 5, stdin);
        char theta[5];
        fgets(theta, 5, stdin);
        
        float x_f = *(float *)x;
        float y_f = *(float *)y;
        float theta_f = *(float *)theta;

        this->location = Coordinate(x_f, y_f, theta_f);
        return true;
    } else {
        return false;
    }
}

void VisionSystemClient::print(const char *message) {
    // do what we want
    if(this->init) {
        int s_len = strlen(message);
        if(s_len <= 255) {
            fputc('\x02', stdout);
            fputc((char)s_len, stdout);
            fputs(message, stdout);
            fflush(stdout);
            
            while(fgetc(stdin) != '\x08');
        }
    }
}

void VisionSystemClient::print(int message) {
    // do what we want
    if(this->init) {
        char str[256];
        sprintf(str, "%d", message);
        fputc('\x02', stdout);
        fputs(str, stdout);
        fflush(stdout);
        
        while(fgetc(stdin) != '\x08');
    }
}

void VisionSystemClient::print(double message) {
    // do what we want
    if(this->init) {
        char str[256];
        sprintf(str, "%f", message);
        fputc('\x02', stdout);
        fputs(str, stdout);
        fflush(stdout);
        
        while(fgetc(stdin) != '\x08');
    }
}

void VisionSystemClient::println(const char *message) {
    // do what we want
    if(this->init) {
        this->print(message);
        this->print("\n");
    }
}

void VisionSystemClient::println(int message) {
    // do what we want
    if(this->init) {
        this->print(message);
        this->print("\n");
    }
}

void VisionSystemClient::println(double message) {
    // do what we want
    if(this->init) {
        this->print(message);
        this->print("\n");
    }
}

void delay(int msec) {
    // do what we want
    fputc('\x07', stdout);
    fputc((char)(msec), stdout);
    fputc((char)(msec >> 8), stdout);
    fputc((char)(msec >> 16), stdout);
    fputc((char)(msec >> 24), stdout);
    fflush(stdout);
    
    while(fgetc(stdin) != '\x08');
}