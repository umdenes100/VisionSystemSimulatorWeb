#ifndef TANK_H
#define TANK_H

#define M1SpeedPin 5
#define M2SpeedPin 6
#define M1DirectionPin 4
#define M2DirectionPin 7

class Tank
{
public:
  Tank();
  void begin();
  float readDistanceSensor(int ln, int id);
  void turnOffMotors(int ln);
  void setRightMotorPWM(int ln, short pwm);
  void setLeftMotorPWM(int ln, short pwm);
private:
  bool init = false;
};

#endif