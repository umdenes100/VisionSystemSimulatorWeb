
# For Staff: Simulator Backend Maintenance Manual

## Docker

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

<!-- ## Testing

We've got support for extensive and flexible testing. To create and modify tests - simply fill out and add to the tests in the tests directory. To create a new tests file, just prepend the name with `test_` to ensure that it will run.

To run all the tests - 



```bash
chmod +rwx run_tests.sh
./run_tests
``` -->

## Maintenance
If the simulator is taking more than a few seconds to simulate, it may need some love.

### First thing to try: Restart it.

Steps to restart:
1. Make sure your SSH profile is set up with the right credentials.
Talk to Hannah for help with this.
2. Open a terminal.
3. Type the following commands:

```
ssh prodSim
cd VisionSystemSimulatorWeb
sudo docker-compose stop
sudo docker-compose up --build &>/dev/null & 
disown
logout
```

What you are doing here is:
- sshing into the system where the simulator backend is hosted
- stopping the currently running docker container
- starting the new docker container and suppressing the output
- disowning it to let it keep running after you exit (not sure if necessary but can't hurt)
- exiting

### Second thing to try: Clear the environments folder.

A known issue with the simulator is that it doesn't delete the simulation environments after simulating like it's supposed to. Because of this, the environments folder currently needs to be regularly manually cleared out. 

(If you can figure out how to have the environments delete after the simulation is finished being generated, that would remove the need for this!)

Steps to clear environments folder:
1. Make sure your SSH profile is set up with the right credentials.
Talk to Hannah for help with this.
2. Open a terminal.
3. Type the following commands:

```
ssh prodSim
cd VisionSystemSimulatorWeb
sudo docker-compose stop
cd server/simulator/environments
sudo rm -rf *
sudo docker-compose up --build &>/dev/null & 
disown
logout
```
What you are doing here is:
- sshing into the system where the simulator backend is hosted
- stopping the currently running docker container
- forcibly removing everything in the environments folder
- starting the new docker container and suppressing the output
- disowning it to let it keep running after you exit (not sure if necessary but can't hurt)
- exiting

If it's been a long time since the environments folder was cleared, ``sudo rm -rf *`` might not work because there is too much stuff.

In this case, you can ``sudo rm -rf a*``, ``sudo rm -rf b*``, etc. until everything is cleared out.

### Debugging

Sometimes, the simulator will do a weird thing and you won't know why it's doing it. In this section I will tell you everything I know about debugging the simulator, which unfortunately isn't a whole lot.

The simulator recieves requests from the client in ``middleware``, which is python, then does most of the dirty work in a C program.
The output of the C program, which is animation data, is returned to ``middleware`` and sent back to the client.

If you want to use ``print`` in ``middleware`` to see what the simulator is doing in the middleware, just leave out the ``&>/dev/null &`` so that the output is not suppressed. (So your command to start the docker container will just be ``sudo docker-compose up --build``.)

Unfortunately, this doesn't seem to show ``printf`` statements in the C part of the program. I'm not sure where those go.
