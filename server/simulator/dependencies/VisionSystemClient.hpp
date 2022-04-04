#ifndef VisionSystemClient_hpp
#define VisionSystemClient_hpp

class Coordinate
{
public:
  Coordinate();
  Coordinate(double x, double y);
  Coordinate(double x, double y, double theta);

  double x;
  double y;
  double theta;

private:
  void init(double x, double y, double theta);
};

class VisionSystemClient
{
public:
  bool ping();
  bool begin(int ln, const char *teamName, int teamType, int markerId, int rxPin, int txPin);
  bool updateLocation(int ln);
  bool mission(int ln, int message);
  bool mission(int ln, double message);
  bool mission(int ln, const char *message);
  bool mission(int ln, Coordinate &message);
  void print(int ln, const char *message);
  void print(int ln, int message);
  void print(int ln, double message);
  void println(int ln, const char *message);
  void println(int ln, int message);
  void println(int ln, double message);

  Coordinate location;
  Coordinate missionSite;

private:
  bool init = false;
};

void delay(int ln, int msec);

#endif /* VisionSystemClient_hpp */