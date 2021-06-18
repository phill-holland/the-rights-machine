set(BINARY ${CMAKE_PROJECT_NAME}_tst)

file(GLOB_RECURSE TEST_SOURCES LIST_DIRECTORIES false *.h *.cpp)

set(SOURCES ${TEST_SOURCES})

enable_testing()

add_executable(${BINARY} ${TEST_SOURCES})

add_test(NAME ${BINARY} COMMAND ${BINARY})

#target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest_main)
target_link_libraries(${BINARY} PUBLIC ${CMAKE_PROJECT_NAME}_lib gtest_main)

include(GoogleTest)
gtest_discover_tests(${CMAKE_PROJECT_NAME}_tst)


#target_link_libraries(${PROJECT_NAME} gtest_main)
#target_link_libraries(hello_test gtest_main)

#include(GoogleTest)
#gtest_discover_tests(hello_test)
