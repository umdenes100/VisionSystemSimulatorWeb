#ifndef TANK_H
#define TANK_H

#define M1SpeedPin 5
#define M2SpeedPin 6
#define M1DirectionPin 4
#define M2DirectionPin 7

class Tank {
  public:
    Tank();
    void begin();

    void turnOffMotors();
    void setRightMotorPWM(int pwm);
    void setLeftMotorPWM(int pwm);
};


#endif