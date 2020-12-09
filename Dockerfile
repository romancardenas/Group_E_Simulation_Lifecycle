FROM ubuntu:21.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update
RUN apt-get -qq upgrade
RUN apt-get -qq install cmake build-essential make git
RUN apt-get install -yq libboost-all-dev
RUN mkdir /simulation_lifecycle

COPY . /simulation_lifecycle/
RUN cd /simulation_lifecycle && ./setup.sh -b

ENTRYPOINT ls /simulation_lifecycle/bin && cd /simulation_lifecycle/build && make test
