#ifndef ENES100_H
#define ENES100_H

#include "VisionSystemClient.hpp"
#include "ArduinoHelpers.hpp"

#define INPUT 0
#define OUTPUT 0 
#define LOW 0
#define HIGH 0

#define BLACK_BOX   0
#define CHEMICAL    1
#define DEBRIS      2
#define FIRE        3
#define WATER       4

#define FRESH       0
#define POLLUTED    1
#define SALT        2

#define COPPER      0
#define STEEL       1

VisionSystemClient Enes100;
SerialClient Serial;

#endif