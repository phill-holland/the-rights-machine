FROM nvidia/cuda:11.2.1-devel-ubuntu20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install build-essential wget -y
RUN apt-get update && apt-get install unixodbc-dev -y
RUN apt-get update && apt-get install libssl-dev -y
RUN apt-get update && apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev -y
RUN apt-get update && apt-get install gdb -y
RUN apt-get update && apt-get install uuid-dev -y
RUN apt-get update && apt-get install git-all -y
RUN apt-get update && apt-get install libfmt-dev -y
WORKDIR /init
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.79.0/source/boost_1_79_0.tar.gz
RUN tar xvzf boost_1_79_0.tar.gz
WORKDIR /init/boost_1_79_0
RUN ./bootstrap.sh
RUN ./b2 install
WORKDIR /init
RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.sh
RUN sh cmake-3.18.4-Linux-x86_64.sh --skip-license
RUN ln -s /init/bin/cmake /usr/local/bin/cmake
