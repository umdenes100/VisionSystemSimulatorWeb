#!/bin/sh
sudo docker-compose stop
cd simulator/environments
sudo rm -rf *
sudo docker-compose up --build &>/dev/null & 
