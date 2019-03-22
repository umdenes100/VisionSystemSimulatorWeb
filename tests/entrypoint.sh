#!/bin/bash

cd /server
python3 middleware.py &

cd /tests
python3 -u test_middleware.py
