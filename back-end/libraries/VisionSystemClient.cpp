#include "VisionSystemClient.hpp"

Coordinate::Coordinate()
{
    init(0, 0, 0);
}

Coordinate::Coordinate(double x, double y)
{
    init(x, y, 0);
}

Coordinate::Coordinate(double x, double y, double theta)
{
    init(x, y, theta);
}

void Coordinate::init(double x, double y, double theta)
{
    this->x = x;
    this->y = y;
    this->theta = theta;
}

bool VisionSystemClient::ping()
{
    // do what we want
}

bool VisionSystemClient::begin(const char *teamName, int teamType, int markerId, int rxPin, int txPin)
{
    // do what we want
}

bool VisionSystemClient::updateLocation()
{
    // do what we want
}

bool VisionSystemClient::mission(int message)
{
    // do what we want
}

bool VisionSystemClient::mission(double message)
{
    // do what we want
}

bool VisionSystemClient::mission(Coordinate &message)
{
    // do what we want
}

void VisionSystemClient::print(const char *message)
{
    // do what we want
}

void VisionSystemClient::print(int message)
{
    // do what we want
}

void VisionSystemClient::print(double message)
{
    // do what we want
}

void VisionSystemClient::println(const char *message)
{
    // do what we want
}

void VisionSystemClient::println(int message)
{
    // do what we want
}

void VisionSystemClient::println(double message)
{
    // do what we want
}

bool VisionSystemClient::receive(Coordinate *coordinate)
{
    // do what we want
}
