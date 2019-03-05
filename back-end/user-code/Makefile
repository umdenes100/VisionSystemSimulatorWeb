CC=g++

all: ArduinoHelpers.o Tank.o VisionSystemClient.o Enes100.h
	$(CC) -o $(name) $(name).cpp ArduinoHelpers.o Tank.o VisionSystemClient.o

ArduinoHelpers.o: ArduinoHelpers.hpp ArduinoHelpers.cpp
	$(CC) -c ArduinoHelpers.cpp

Tank.o: Tank.h Tank.cpp
	$(CC) -c Tank.cpp

VisionSystemClient.o: VisionSystemClient.cpp VisionSystemClient.hpp
	$(CC) -c VisionSystemClient.cpp