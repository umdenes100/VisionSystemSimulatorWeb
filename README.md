# VisionSystemSimulatorWeb

## For Students

The simulator is a tool designed to give you a head start on programming and to help you learn how to program. To start off, we will look at the layout of the simulator. After that we will go into depth about the Enes100 and Tank simulator libraries and some small notes regarding coding in the simulator.

### Layout

The simulator is divided into two modes: development and simulation. In the development half of the simulator you will see the code editor, a few action buttons, an arena preview, and an OSV editor. The code editor comes with a 'Terminal' which allows you to see the status of your simulations and also prints the error message if your code fails to compile. The action buttons allow you to (1) see the documentation (this page) (2) randomize the arena, which will be visible on the preview, (3) perform a simulation, and (4) add in or remove obstacles from the arena. In the arena preview you will see the OSV location, the obstacle locations, the destination location, and the size of the OSV. The size of the OSV can be edited in the OSV editor, along with the distance sensors, which will be covered in greater detail later.

The Simulation section has only 3 elements, the code highlighting, the output console, and the visual simulation. The code highlighter will take the 5 most recent ENES100 or TANK function calls and highlight them. This should allow you to debug issues quickly and painlessly. The output console simply prints out all Enes100.print and Enes100.println calls. Finnally, the actual simulation will show a video simulation of your code.

### Libraries

The simulator uses a variant of the Enes100 and Tank libraries provided for the course. The libraries are largely the same save a few exceptions which we will mention later. Note that because of technical limitations, some functions available in ardunio such as `digitalRead()` and `Serial.print()` are unavailable in the simulator and will prevent compilation. Below are the functions provided in the libraries.

#### Enes100

```Enes100::begin(char *, int, int, int, int)```
This function initializes the Enes100 object and is required in order to utilize any other Enes100 related functions in the code. It also sets the Enes100.destination.x, y, and theta variables which are used to locate the destination in the arena.

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
This function is used to get distance data of the obstacles in the field. The distance is returned in meters, and is calculated by drawing a straight line from the distance sensor to the first object it hits. The distance reported will max out at 1m, and if the sensor is not enabled, it will return -1. To enable a sensor, go to the OSV editor, and select the sensors you wish to add by clicking on them when they darken on the drawing. The sensorId corresponds to the sensors location on the OSV, and is labeled below:

![alt text](img.png "Distance Sensors")


## For Staff: Docker

Given that we have developers using every operating system under the sun, we have to have some way of standardizing things without making life hell for everybody. Enter Docker.

### Setup

The first thing we're going to do is make life hell for Windows users. There are multiple Docker solutions on Windows (Docker Toolbox for Windows, Docker for Windows) but only one that will not make you want to burn your computer and become a hermit.

You'll want Docker for Windows. Unfortunately, this uses a hypervisor known as HyperV. It isn't compatible with Windows 10 Home. Fortunately, the university gives away 
Windows 10 Education, which will do just fine. (This will break VirtualBox. Talk to Karam about how to fix it.)

Once that's installed, install Docker for Windows. Macintosh users, install Docker for Mac.

### Background

Docker utilizes a technology known as containerization. Little isolated environments that share a kernel but nothing else. We have two containers in this application - client and server. Each container is set up using a Dockerfile, which is a glorified bash script.

You can build these containers into static images for convenience, and even extend other people's static containers for your own use. For example, the client container extends an NGINX image and the server container extends an Ubuntu image.

These containers are orchestrated using docker-compose. In our configuration file (docker-compose.yml), you can see a few features of the orchestration, notably the names of the containers, the ports within the containers that are mapped to ports on your machine, and the directories that have been mounted. Mounting directories sets up your container so that certain directories on the container are tied to local directories on your machine and will reflect changes. For example, the client directory is mounted to NGINX's serving directory, which means that if you change a client file, a browser refresh will show you the changes you made.

### Use

Docker and docker-compose are well documented and so I won't talk about too many commands, but here are some key useful ones to know.

`docker-compose up`

Creates all your containers.

`docker-compose up -d`

Creates all your containers and then detaches so that you can continue to use your terminal.

`docker-compose up --force-recreate --build`

Recreates the container and rebuilds with any changes you might have made. Useful for when you modify the server. Docker caches steps so although it takes a while the first time you run it, subsequent runs will be quicker.

`docker-compose exec <container-name> bash`

Opens up the container and allows you to explore it in its current state. Useful for trying stuff out and debugging. e.g. `docker-compose exec server bash`

`docker-compose stop`

Stops all containers.

`docker-compose kill`

Stops all containers, violently.

## Testing

We've got support for extensive and flexible testing. To create and modify tests - simply fill out and add to the tests in the tests directory. To create a new tests file, just prepend the name with `test_` to ensure that it will run.

To run all the tests - 

```bash
chmod +rwx run_tests.sh
./run_tests
```

