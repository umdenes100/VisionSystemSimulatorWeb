#ifndef ENES100_H
#define ENES100_H

#define INPUT 0
#define OUTPUT 0
#define LOW 0
#define HIGH 0

void pinMode(int pin, int mode);
void digitalWrite(int pin, int val);
int digitalRead(int pin);
void delay(int msec);

#endif