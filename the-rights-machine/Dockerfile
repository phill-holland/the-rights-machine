FROM nvidia/cuda:11.8.0-devel-ubuntu22.04 as build
ENV DEBIAN_FRONTEND=noninteractive
RUN apt-get update && apt-get install build-essential wget -y
RUN apt-get update && apt-get install unixodbc-dev -y
RUN apt-get update && apt-get install libssl-dev -y
RUN apt-get update && apt-get install gdb -y
RUN apt-get update && apt-get install libfmt-dev uuid-dev -y
RUN apt-get update && apt-get install curl -y
RUN curl -s --compressed "https://phill-holland.github.io/ppa/KEY.gpg" | gpg --dearmor | tee /etc/apt/trusted.gpg.d/my_ppa.gpg >/dev/null
RUN curl -s --compressed -o /etc/apt/sources.list.d/my_list_file.list "https://phill-holland.github.io/ppa/my_list_file.list"
RUN apt-get update && apt-get install cplusplus-core -y
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
WORKDIR /code
COPY . .
WORKDIR /code/build
RUN rm -rfv
WORKDIR /code
RUN sed -i -r "s/Debug/Release/i" CMakeLists.txt
RUN cmake .
RUN make

FROM nvidia/cuda:11.8.0-runtime-ubuntu22.04 AS deploy
EXPOSE 5454
COPY --from=build /usr/lib/x86_64-linux-gnu/libodbc.so.2 /usr/lib/x86_64-linux-gnu/libodbc.so.2
COPY --from=build /usr/lib/x86_64-linux-gnu/libltdl.so.7 /usr/lib/x86_64-linux-gnu/libltdl.so.7
COPY --from=build /usr/local/lib/libboost_json.so.1.79.0  /usr/local/lib/libboost_json.so.1.79.0  
COPY --from=build /usr/lib/libcplusplus-core.so /usr/lib/libcplusplus-core.so
ENV LD_LIBRARY_PATH=/usr/lib:$LD_LIBRARY_PATH
WORKDIR /machine
COPY --from=build /code/build/source/the-rights-machine .
CMD [ "/machine/./the-rights-machine" ]