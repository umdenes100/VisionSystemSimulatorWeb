#include "ArduinoHelpers.hpp"

SerialClient::SerialClient() {
    // do nothing
}

void SerialClient::begin(int baud) {
    // do what we want
}

void SerialClient::print(char *string) {
    // do what we want
}

void SerialClient::println(char *string) {
    // do what we want
}

// this is where we will define the arduino specific functions
void pinMode(int pin, int mode) {
    // do what we want
}

void digitalWrite(int pin, int val) {
    // do what we want
}

int digitalRead(int pin) {
    // do what we want
}

void delay(int msec) {
    // do what we want
}