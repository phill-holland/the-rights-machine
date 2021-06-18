cmake_minimum_required(VERSION 3.18)

project(the-rights-machine)

set(CMAKE_CUDA_COMPILER "/usr/local/cuda/bin/nvcc")
set(CMAKE_CUDA_COMPILER_ENV_VAR "")
set(CMAKE_CUDA_FLAGS "${CMAKE_CUDA_FLAGS} -Wno-deprecated-gpu-targets")

set(CMAKE_BUILD_TYPE "Debug")

enable_language(CUDA)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
find_package(OpenSSL REQUIRED)
find_package(OpenGL REQUIRED)
find_package(X11 REQUIRED)

include_directories("/usr/local/cuda/include")
include_directories("/usr/include")
include_directories("source/include")
include_directories("source/cuda include")

file(GLOB SOURCE CONFIGURE_DEPENDS "${PROJECT_SOURCE_DIR}/source/source/*.cpp" "${PROJECT_SOURCE_DIR}/source/cuda source/*.cu")

link_directories("/usr/local/cuda/lib64")

add_executable(${PROJECT_NAME} ${SOURCE})

set_property(TARGET ${PROJECT_NAME} PROPERTY CUDA_ARCHITECTURES 50 61)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} "${OPENGL_LIBRARIES}")
target_link_libraries(${PROJECT_NAME} X11)
target_link_libraries(${PROJECT_NAME} odbc)

