cmake_minimum_required(VERSION 3.7)
project(algorithm_lib_cpp14)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++14 -O2 -Wextra -pedantic -Wformat=2")

file(GLOB_RECURSE HEADERS "lib/*.h")

add_library(lib INTERFACE)
target_include_directories(lib INTERFACE .)
target_sources(lib INTERFACE ${HEADERS})

#set(SOURCE_FILES "tests/main.cpp")
#add_executable(tests ${SOURCE_FILES})
#target_link_libraries(tests lib)

file(GLOB_RECURSE TESTS "tests/*.?pp")
add_executable(tests ${TESTS})
target_link_libraries(tests lib)

# google-test
add_subdirectory(googletest)
target_link_libraries(tests gtest_main)
target_compile_options(tests PRIVATE -Werror -std=gnu++14)
