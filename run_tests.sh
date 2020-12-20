#!/bin/bash
cd build || { echo "Failed to change directory to build folder"; exit; }
make test
