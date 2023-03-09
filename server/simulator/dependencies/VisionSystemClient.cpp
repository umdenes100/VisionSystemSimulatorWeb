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
    // Do nothing.
}

//need to add an extra parameter here!!!
bool VisionSystemClient::mission(int ln, int type, int message) {
    // Do nothing.
}

bool VisionSystemClient::mission(int ln, int type, double message) {
    // Do nothing.
}

bool VisionSystemClient::mission(int ln, int type, const char *message) {
    // Do nothinng.
}

bool VisionSystemClient::mission(int ln, int type, Coordinate &message) {
    // Do nothing.
}

//ln seems to contain the coordinate set for the mission set
bool VisionSystemClient::begin(int ln, const char *teamName, int teamType, int markerId, int rxPin, int txPin) {
    // Do what we want.
    this->init = true; //set vision system client private var init to be true.
    fputc('\x00', stdout); //seems to be an opcode
    fputc((char)(ln), stdout);
    fputc((char)(ln >> 8), stdout); //each char is 8 bits, so we will meed to shift ln by 8 bits to properly read the next char.
    fputc((char)(ln >> 16), stdout);
    fputc((char)(ln >> 24), stdout);
    fflush(stdout); //flush the stdout to standard output


    //somehow get 12 chars in this.
    char x[4]; //why is char array length 4?
    x[0] = fgetc(stdin); x[1] = fgetc(stdin); x[2] = fgetc(stdin); x[3] = fgetc(stdin); //fgetc will get next character from stdandard input
    char y[4];
    y[0] = fgetc(stdin); y[1] = fgetc(stdin); y[2] = fgetc(stdin); y[3] = fgetc(stdin);
    char theta[4];
    theta[0] = fgetc(stdin); theta[1] = fgetc(stdin); theta[2] = fgetc(stdin); theta[3] = fgetc(stdin);

    float x_f = *(float *)x; //pointer shenanigens
    float y_f = *(float *)y;
    float theta_f = *(float *)theta;

    //we extract the coordinates and put them into the missionSite "object"
    this->missionSite = Coordinate(x_f, y_f, theta_f);
    return true;
}

//same logic as the begin statement
bool VisionSystemClient::updateLocation(int ln) {
    // Do what we want.
    if (this->init) {
        fputc('\x01', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fflush(stdout);

        char x[4];
        x[0] = fgetc(stdin); x[1] = fgetc(stdin); x[2] = fgetc(stdin); x[3] = fgetc(stdin);
        char y[4];
        y[0] = fgetc(stdin); y[1] = fgetc(stdin); y[2] = fgetc(stdin); y[3] = fgetc(stdin);
        char theta[4];
        theta[0] = fgetc(stdin); theta[1] = fgetc(stdin); theta[2] = fgetc(stdin); theta[3] = fgetc(stdin);

        float x_f = *(float *)x;
        float y_f = *(float *)y;
        float theta_f = *(float *)theta;

        this->location = Coordinate(x_f, y_f, theta_f);
        return true;
    } else {
        return false;
    }
}

void VisionSystemClient::print(int ln, const char *message) {
    // Do what we want.
    if (this->init) {
        int s_len = strlen(message);
        if (s_len <= 255) {
            fputc('\x02', stdout);
            fputc((char)(ln), stdout);
            fputc((char)(ln >> 8), stdout);
            fputc((char)(ln >> 16), stdout);
            fputc((char)(ln >> 24), stdout);
            fputc((char)(s_len + 1), stdout);
            fputs(message, stdout);
            fputc('\x0', stdout); //null termination?
            fflush(stdout);

            while (fgetc(stdin) != '\x08');
        }
    }
}

void VisionSystemClient::print(int ln, int message) {
    // Do what we want.
    if (this->init) {
        char str[256];
        sprintf(str, "%d", message);
        fputc('\x02', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(strlen(str) + 1), stdout);
        fputs(str, stdout);
        fputc('\x0', stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');
    }
}

void VisionSystemClient::print(int ln, double message) {
    // Do what we want.
    if (this->init) {
        char str[256];
        sprintf(str, "%f", message);
        fputc('\x02', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(strlen(str) + 1), stdout);
        fputs(str, stdout);
        fputc('\x0', stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');
    }
}

void VisionSystemClient::println(int ln, const char *message) {
    // Do what we want.
    if (this->init) {
        int s_len = strlen(message);
        if (s_len <= 254) {
            char str[256];
            sprintf(str, "%s\n", message);
            fputc('\x02', stdout);
            fputc((char)(ln), stdout);
            fputc((char)(ln >> 8), stdout);
            fputc((char)(ln >> 16), stdout);
            fputc((char)(ln >> 24), stdout);
            fputc((char)(s_len + 2), stdout);
            fputs(str, stdout);
            fputc('\x0', stdout);
            fflush(stdout);

            while (fgetc(stdin) != '\x08');
        }
    }
}

void VisionSystemClient::println(int ln, int message) {
    // Do what we want.
    if (this->init) {
        char str[256];
        sprintf(str, "%d\n", message);
        fputc('\x02', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(strlen(str) + 2), stdout);
        fputs(str, stdout);
        fputc('\x0', stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');
    }
}

void VisionSystemClient::println(int ln, double message) {
    // Do what we want.
    if (this->init) {
        char str[256];
        sprintf(str, "%f\n", message);
        fputc('\x02', stdout);
        fputc((char)(ln), stdout);
        fputc((char)(ln >> 8), stdout);
        fputc((char)(ln >> 16), stdout);
        fputc((char)(ln >> 24), stdout);
        fputc((char)(strlen(str) + 2), stdout);
        fputs(str, stdout);
        fputc('\x0', stdout);
        fflush(stdout);

        while (fgetc(stdin) != '\x08');
    }
}

void delay(int ln, int msec) {
    // Do what we want.
    fputc('\x07', stdout);
    fputc((char)(ln), stdout);
    fputc((char)(ln >> 8), stdout);
    fputc((char)(ln >> 16), stdout);
    fputc((char)(ln >> 24), stdout);
    fputc((char)(msec), stdout);
    fputc((char)(msec >> 8), stdout);
    fputc((char)(msec >> 16), stdout);
    fputc((char)(msec >> 24), stdout);
    fflush(stdout);

    while (fgetc(stdin) != '\x08');
}