FROM nvidia/cuda:11.2.1-devel-ubuntu20.04
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install build-essential wget -y
RUN apt-get update && apt-get install gcc-10 g++-10 -y
RUN update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-10 10
RUN update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-10 10
RUN apt-get update && apt-get install unixodbc-dev -y
RUN apt-get update && apt-get install libssl-dev -y
RUN apt-get update && apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev -y
RUN apt-get update && apt-get install gdb -y
WORKDIR /init
RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.sh
RUN sh cmake-3.18.4-Linux-x86_64.sh --skip-license
RUN ln -s /init/bin/cmake /usr/local/bin/cmake
