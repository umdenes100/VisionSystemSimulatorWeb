#!/bin/sh

echo "Building randomization binary..." && \
make -C /server/randomization clean && \
make -C /server/randomization

echo "Building simulator binary..." && \
make -C /server/simulator/simulator clean && \
make -C /server/simulator/simulator

python3.7 -u middleware.py
