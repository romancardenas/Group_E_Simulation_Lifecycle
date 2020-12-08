#!/bin/bash

# Check that the number of arguments is correct
if [ "$#" -gt 1 ]
then
  echo "Illegal number of parameters"
  echo "Usage: ./setup.sh [build | b | -b]"
  exit
fi

# Download and update all Git submodules
git submodule update --init --recursive

# compile Cadmium Cell-DEVS models
cd third_party/CellDEVS_models/tutorial || { echo "Failed to change directory to Cell-DEVS tutorial folder"; exit; }
mkdir build
cd build || { echo "Failed to change directory to build folder"; exit; }
cmake ..
make

cd ../../../..
mkdir logs

# Depending on the first argument, setup.sh may build the project.
case "$1" in
  build|b|-b)
    bash build.sh
    ;;
esac
