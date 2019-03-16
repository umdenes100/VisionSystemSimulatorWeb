let connection = new WebSocket("ws://0.0.0.0:8888/")

let request = `
#include "Enes100.h"
#include "Tank.h"
#include <iostream>

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
    std::cout << "hello";
    Enes100.updateLocation();
    tank.setLeftMotorPWM(200);
    for(int i = 0; i < 10; i++) {
        Serial.println(string);
    }
}

void function( int wow, char c) {
    return;
}
`

console.log(request)

connection.onopen = () => {
	console.log('OPEN')
	connection.send(request)
}

connection.onerror = error => {
    console.log('WebSocket Error.')
}

connection.onmessage = message => {
	console.log(message.data)
}

connection.onclose = () => {
	console.log('Failed')
}
