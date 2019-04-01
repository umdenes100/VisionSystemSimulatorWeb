#!/bin/bash

cd /server
python3.7 middleware.py &

cd /tests
pytest

rm -rf .pytest_cache
rm -rf __pycache__
