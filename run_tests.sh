#!/bin/bash

chmod +rwx tests/entrypoint.sh

docker-compose -f docker-compose.test.yml up --force-recreate --build
docker-compose -f docker-compose.test.yml kill
