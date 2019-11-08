#ifndef ArduinoHelpers_hpp
#define ArduinoHelpers_hpp

#include "Tank.h"
#include "VisionSystemClient.hpp"

class SerialClient
{
public:
  SerialClient();
  void begin(int baud);
  void print(char *string);
  void println(char *string);
};

void pinMode(int pin, int mode);
void digitalWrite(int pin, int val);
int digitalRead(int pin);

#endif