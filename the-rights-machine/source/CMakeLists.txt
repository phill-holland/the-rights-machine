set(BINARY ${CMAKE_PROJECT_NAME})

file(GLOB_RECURSE SOURCES LIST_DIRECTORIES true *.h *.cpp)

set(SOURCES ${SOURCES})

link_directories("/usr/local/cuda/lib64")

add_executable(${BINARY} ${SOURCES})

add_library(${BINARY}_lib STATIC ${SOURCES})

set_property(TARGET ${PROJECT_NAME} PROPERTY CUDA_ARCHITECTURES 50 61)
set_property(TARGET ${PROJECT_NAME} PROPERTY CXX_STANDARD 20)

target_link_libraries(${PROJECT_NAME} "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${PROJECT_NAME} OpenSSL::SSL)
target_link_libraries(${PROJECT_NAME} OpenSSL::Crypto)
target_link_libraries(${PROJECT_NAME} X11)