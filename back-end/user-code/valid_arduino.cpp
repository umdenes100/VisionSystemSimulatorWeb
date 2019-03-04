#include "Enes100.h"
#include "Tank.h"

int wow = 3;

char *string = "wowwowowowo";

void setup() {
    pinMode(3, OUTPUT);
    digitalWrite(3, LOW);
    pinMode(4, INPUT);
    digitalRead(4);
    Serial.begin(9600);
    delay(20);
}

void loop() {
    Serial.println("wow");
    Enes.updateLocation();
    Tank.setLeftMotorPWM(200);
    for(int i = 0; i < 10; i++) {
        Serial.println("wowee");
    }
}

void function() {
    return;
}