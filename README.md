# VisionSystemSimulatorWeb

The simulator is a tool designed to give you a head start on programming and to help you learn how to program using the Vision System. To start off, we will look at the layout of the simulator. After that we will go into depth about the Enes100 and Tank simulator libraries and some small notes regarding coding in the simulator.

## Layout

The simulator is divided into two modes: development and simulation. In the development half of the simulator you will see the code editor, a few action buttons/checkboxes, an arena preview, and an OSV editor. The Simulation section has three elements: the code highlighting, the output console, and the visual simulation. 

### Development

The code editor comes with a 'Terminal' which allows you to see the status of your simulations and also prints the error message if your code fails to compile. The action buttons allow you to 
1. See the documentation (this page). 
2. Randomize the arena, which will be visible on the preview.
3. Perform a simulation.
4. Save and download a copy of your current code.
5. Access example code.
6. Alert the teaching fellows of any issues with the simulator (hopefully this is unnessesary). 
Additionally there are two checkboxes that allow you to 
1. Put the screen in dark mode.
2. Add or remove obstacles from the arena. 
In the arena preview you will see the OSV location, the obstacle locations, the mission site location, and the size of the OSV. The size of the OSV can be edited in the OSV editor. There are also 9 ultrasonic distance sensors that can be used to help detect obstacles.

[![Simulator Image Capture](https://raw.githubusercontent.com/umdenes100/VisionSystemSimulatorWeb/master/images/DevelopmentSection.jpg "The Vision System Simulator (Click me)")](http://enes100.umd.edu/simulatorweb)

### Simulation

The code highlighter will take the five most-recent ENES100 or TANK function calls and highlight them. This should allow you to debug issues quickly and painlessly. The output console simply prints out all Enes100.print and Enes100.println calls. Finally, the actual simulation will show a video simulation of your code.

[![Simulator Image Capture](https://raw.githubusercontent.com/umdenes100/VisionSystemSimulatorWeb/master/images/SimulationSection.jpg "The Vision System Simulator (Click me)")](http://enes100.umd.edu/simulatorweb)


### Libraries

The simulator uses a variant of the Enes100 and Tank libraries provided for the course. The libraries are largely the same save a few exceptions which we will mention later. Note that because of technical limitations, some functions available in ardunio such as `digitalRead()` and `Serial.print()` are unavailable in the simulator and will prevent compilation. Below are the functions provided in the libraries.

#### Enes100

```Enes100::begin(char *, int, int, int, int)```
This function initializes the Enes100 object and is required in order to utilize any other Enes100 related functions in the code. It also sets the Enes100.missionSite.x, y, and theta variables which are used to locate the mission site in the arena.

```Enes100::updateLocation()```
This function updates the Enes100.location.x, y, and theta variables in the object to the latest location of the OSV in the simulation.

```Enes100::print(arg)```
This function will print whatever is passed in. The argument can either be an int, float, double, or char *.

```Enes100::println(arg)```
This function will print whatever is passed in followed by a new line character.

```delay(int msec)```
This function is not a member of the Enes100 class, but can be called by simply typing `delay(arg)` where arg is the number of milliseconds you would like to stop code execution.

#### Tank

```Tank::begin()```
This function initalizes the tank object and is required in order to utilize any other Tank related function in the code.

```Tank::setLeftMotorPWM(int pwm)```
This function sets the PWM of the left motors in the OSV. This value can be anywhere from -255 to 255, with -255 being full speed backwards and 255 being full speed forwards.

```Tank::setRightMotorPWM(int pwm)```
This function sets the PWM of the right motors in the OSV. This value can be anywhere from -255 to 255, with -255 being full speed backwards and 255 being full speed forwards.

```Tank::turnOffMotors()```
This function simply stops the OSV where it is

```Tank::readDistanceSensor(int sensorId)```
This function is used to get distance data of the obstacles in the field. The distance is returned in meters, and is calculated by drawing a straight line from the distance sensor to the first object it hits. The distance reported will max out at 1m, and if the sensor is not enabled, it will return -1. To enable a sensor, go to the OSV editor, and select the sensors you wish to add by clicking on them when they darken on the drawing. The sensorId value corresponding to each sensor placement on the OSV is labeled below:

[<img src="https://raw.githubusercontent.com/umdenes100/VisionSystemSimulatorWeb/master/images/simulator_distance_sensor_placements.png" alt="Possible Distance Sensor Placements on Simulation" title="Possible Distance Sensor Placements on Simulation" width="500">](http://github.com/umdenes100/VisionSystemSimulatorWeb/blob/master/images/simulator_distance_sensor_placements.png)

#### Other libraries

You can import other standard Arduino libraries by including them at the top.

For example, to include the Arduino library `math.h`, you would write: `#include <math.h>` at the top of your code, which would allow you to use commands like `atan()`.

Note that the angle brackets`<>` indicate you are including a system library, and quotes `""` indicate you are including a local library.
