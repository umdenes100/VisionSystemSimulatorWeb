#ifndef State_hpp
#define State_hpp

#include "VisionSystemClient.hpp"
#include "ArduinoHelpers.hpp"
#include "Tank.h"

class State
{
public:
  void init(char *json);
  void update(VisionSystemClient Enes100, Tank tank, SerialClient Serial);
};

void delay(int msec);

#endif /* State.hpp */
