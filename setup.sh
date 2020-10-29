#!/bin/bash
# shellcheck disable=SC2164

# Download and update all Git submodules
git submodule update --init --recursive

# install cJSON library
cd third_party/cJSON
mkdir build
cd build
cmake .. -Wno-dev -DCMAKE_INSTALL_PREFIX="${PWD}"/..
make
make install

# compile Cadmium Cell-DEVS models TODO
cd ../../CellDEVS_models
