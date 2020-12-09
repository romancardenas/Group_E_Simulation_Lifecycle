FROM ubuntu:21.04

ARG DEBIAN_FRONTEND=noninteractive

RUN apt-get -qq update
RUN apt-get -qq upgrade
RUN apt-get -qq install cmake build-essential make git
RUN apt-get install -yq libboost-all-dev
RUN mkdir /simulation_lifecycle

COPY . /simulation_lifecycle/
RUN rm -rf /simulation_lifecycle/third_party
RUN cd /simulation_lifecycle && ./setup_docker.sh && ./build.sh

ENTRYPOINT ls /simulation_lifecycle/bin && cd /simulation_lifecycle && ./run_tests.sh
