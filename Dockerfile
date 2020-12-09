FROM ubuntu:21.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update
RUN apt-get -qq upgrade
RUN apt-get -qq install cmake build-essential make
RUN apt-get install -yq libboost-all-dev
RUN mkdir /simulation_lifecycle

COPY CMakeLists.txt /simulation_lifecycle/CMakeLists.txt
COPY include /simulation_lifecycle/include
COPY src /simulation_lifecycle/src
COPY test /simulation_lifecycle/test
COPY third_party /simulation_lifecycle/third_party

RUN cd /simulation_lifecycle/third_party/CellDEVS_models/tutorial && rm -rf build && mkdir build && cd build && cmake .. && make
RUN mkdir /simulation_lifecycle/logs && mkdir /simulation_lifecycle/build && cd /simulation_lifecycle/build && cmake .. && make

ENTRYPOINT ls /simulation_lifecycle && cd /simulation_lifecycle/build && make test
