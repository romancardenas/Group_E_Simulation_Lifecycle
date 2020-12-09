#!/bin/bash

# Check that the number of arguments is correct
if [ "$#" -gt 1 ]
then
  echo "Illegal number of parameters"
  echo "Usage: ./setup.sh [build | b | -b]"
  exit
fi

mkdir third_party
cd third_party || { echo "Failed to change directory to third-party folder"; exit; }
git clone --depth 1 --branch celldevs-v0.1 https://github.com/SimulationEverywhere/cadmium.git cadmium
cd cadmium || { echo "Failed to change directory to Cadmium folder"; exit; }
git submodule update --init --recursive
cd ..
git clone --depth 1 --branch v1.7.14 https://github.com/DaveGamble/cJSON.git cJSON
git clone --depth 1 --branch v2.5.1 https://github.com/ThrowTheSwitch/Unity.git Unity
mkdir CellDEVS_models
cd CellDEVS_models || { echo "Failed to change directory to Cell-DEVS models folder"; exit; }
git clone --depth 1 https://github.com/SimulationEverywhere-Models/Cadmium-CellDEVS-Tutorial.git tutorial

# compile Cadmium Cell-DEVS models
cd tutorial || { echo "Failed to change directory to Cell-DEVS tutorial folder"; exit; }
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
