#!/bin/bash

mkdir build
cd build || { echo "Failed to change directory to build folder"; exit; }
cmake ..
make
