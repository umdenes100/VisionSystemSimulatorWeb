#!/bin/sh
sudo docker-compose stop
cd /home/ubuntu/VisionSystemSimulatorWeb/server/simulator/environments
sudo rm -rf *
sudo docker-compose up --build &>/dev/null & 

