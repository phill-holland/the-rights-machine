set(BINARY ${CMAKE_PROJECT_NAME}_test)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

enable_testing()

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${CMAKE_PROJECT_NAME}_test)

target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib "${CMAKE_THREAD_LIBS_INIT}")
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib OpenSSL::SSL)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib OpenSSL::Crypto)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib X11)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib odbc)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib uuid)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib "${Boost_LIBRARIES}")
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib cplusplus-core)