#ifndef Enes100_h
#define Enes100_h

#include "VisionSystemClient.hpp"
#include "ArduinoHelpers.hpp"

#define INPUT             0
#define OUTPUT            0
#define LOW               0
#define HIGH              0

#define BLACK_BOX         0
#define DATA              1
#define DEBRIS            2
#define FIRE              3
#define WATER             4

#define RED               0
#define GREEN             1
#define BLUE              2

#define FRESH_UNPOLLUTED  0
#define FRESH_POLLUTED    1
#define SALT_UNPOLLUTED   2
#define SALT_POLLUTED     3

#define PLASTIC           0
#define ALUMINUM          1

VisionSystemClient Enes100;
SerialClient Serial;

#endif /* Enes100_h */
