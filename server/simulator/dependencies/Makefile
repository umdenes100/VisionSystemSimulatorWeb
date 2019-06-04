CC = g++
CFLAGS = -I ./

all: ArduinoHelpers.o TankClient.o VisionSystemClient.o Enes100.h Tank.h
	$(CC) $(CFLAGS) -o ../environments/$(name)/$(name) ../environments/$(name)/$(name).cpp ArduinoHelpers.o TankClient.o VisionSystemClient.o

ArduinoHelpers.o: ArduinoHelpers.hpp ArduinoHelpers.cpp
	$(CC) -c ArduinoHelpers.cpp

TankClient.o: TankClient.h TankClient.cpp
	$(CC) -c TankClient.cpp

VisionSystemClient.o: VisionSystemClient.cpp VisionSystemClient.hpp
	$(CC) -c VisionSystemClient.cpp

clean:
	rm -rf *~ *.o