#include "Enes100.h"
#include "Tank.h"

void setup() {
	//Initialize the Enes100 and tank objects
	Enes100.begin("Team Name Here", FIRE, 3, 8, 9);
	Tank.begin();
	//Update Location
	Enes100.updateLocation();
	//Print out Destination Coordinates
	Enes100.print("Destination at ");
	Enes100.print(Enes100.destination.x);
	Enes100.print(",");
	Enes100.println(Enes100.destination.y);
	//Print out OSV Coordinates
	Enes100.print("OSV at ");
	Enes100.print(Enes100.location.x);
	Enes100.print(",");
	Enes100.print(Enes100.location.y);
	Enes100.print(",");
	Enes100.println(Enes100.location.theta);
}

void loop() {
	//Drive Forward
	Tank.setLeftMotorPWM(255);
	Tank.setRightMotorPWM(255);
	delay(1000);
	//Turn off Motors
	Tank.turnOffMotors();
	//Turn Left
	Tank.setLeftMotorPWM(-255);
	Tank.setRightMotorPWM(255);
	delay(1000);
	//Turn Right
	Tank.setLeftMotorPWM(255);
	Tank.setRightMotorPWM(-255);
	delay(1000);
	//Turn off Motors
	Tank.turnOffMotors();
	delay(5000);
	Enes100.println(Tank.readDistanceSensor(1));
	//Transmit the number of candles that are lit
	Enes100.mission(4);
}
