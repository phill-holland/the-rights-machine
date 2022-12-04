cmake_minimum_required(VERSION 3.18)

project(the-rights-machine)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
set(CMAKE_CUDA_COMPILER_ENV_VAR "")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Wno-deprecated-gpu-targets")

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_BUILD_TYPE "Debug")

enable_language(CUDA)

set(THREADS_PREFER_PTHREAD_FLAG ON)

find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(X11 REQUIRED)
find_package(Boost 1.79.0 COMPONENTS json REQUIRED)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(cplusplus-core 
GIT_REPOSITORY git@github.com:phill-holland/cplusplus-core.git
GIT_TAG main
)

FetchContent_MakeAvailable(cplusplus-core)

include_directories("source/include")
include_directories("/usr/local/cuda/include")
include_directories("source/cuda include")
include_directories("test/include")
include_directories("${cplusplus-core_SOURCE_DIR}/include")

add_subdirectory(source)
add_subdirectory(test)
