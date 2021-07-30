FROM nvidia/cuda:11.2.1-devel-ubuntu20.04 as build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install build-essential wget -y
RUN apt-get update && apt-get install unixodbc-dev -y
RUN apt-get update && apt-get install libssl-dev -y
RUN apt-get update && apt-get install libglu1-mesa-dev freeglut3-dev mesa-common-dev -y
RUN apt-get update && apt-get install gdb -y
RUN apt-get install uuid-dev -y
WORKDIR /init
RUN wget https://boostorg.jfrog.io/artifactory/main/release/1.76.0/source/boost_1_76_0.tar.gz
RUN tar xvzf boost_1_76_0.tar.gz
WORKDIR /init/boost_1_76_0
RUN ./bootstrap.sh
RUN ./b2 install
WORKDIR /init
RUN wget https://github.com/Kitware/CMake/releases/download/v3.18.4/cmake-3.18.4-Linux-x86_64.sh
RUN sh cmake-3.18.4-Linux-x86_64.sh --skip-license
RUN ln -s /init/bin/cmake /usr/local/bin/cmake
WORKDIR /code
COPY . .
WORKDIR /code/build
RUN rm -rfv
WORKDIR /code
RUN cmake .
RUN make
WORKDIR /code/build/test
RUN the-rights-machine_test

FROM nvidia/cuda:11.1-runtime-ubuntu20.04 AS deploy
EXPOSE 5454
COPY --from=build /code/build/source/the-rights-machine .
CMD [ "the-rights-machine" ]