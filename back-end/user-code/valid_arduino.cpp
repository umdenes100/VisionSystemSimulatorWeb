#include "Enes100.h"
#include "Tank.h"

int wow = 3;

char *string = "wowwowowowo";

Tank tank;

void setup() {
    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);
    pinMode(4, INPUT);
    digitalRead(4);
    Serial.begin(9600);
    tank.begin();
    delay(20);
}

void loop() {
    Serial.println("wow");
    Enes100.updateLocation();
    tank.setLeftMotorPWM(200);
    for(int i = 0; i < 10; i++) {
        Serial.println(string);
    }
}

void function( int wow, char c) {
    return;
}