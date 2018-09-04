cmake_minimum_required(VERSION 3.1)

project(RhombicSweep)

# SET(CMAKE_CXX_COMPILER /usr/bin/g++)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Turn on with 'cmake -Dtest=ON'
option(test "Build all tests." OFF)
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(test ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(RhombicSweep ${SOURCE_FILES})

# Include my files and boost
include_directories(src)
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})
include_directories(include/pcg-cpp-0.98/include)

# Link to my library
set(LIB_FILES src/lattice.h src/lattice.cpp src/code.h src/code.cpp)
add_library(SweepLib ${LIB_FILES})
target_link_libraries(RhombicSweep SweepLib)

if (test)
    enable_testing()
    add_definitions(-DSCTEST)
    # Add my tests
    add_subdirectory(tests)
    # Add googletest
    add_subdirectory(tests/lib/googletest-release-1.8.0/googletest)
    # Include googletest libraries
    include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} src)
    # Add test executables
    add_executable(testLattice tests/test_lattice.cpp)
    add_executable(testCode tests/test_code.cpp)
    # Standard googletest linking
    target_link_libraries(testLattice gtest gtest_main)
    target_link_libraries(testCode gtest gtest_main)
    # Link to my library
    target_link_libraries(testLattice SweepLib)
    target_link_libraries(testCode SweepLib)
    # Enable running tests with 'make test'
    add_test(NAME testLattice COMMAND testLattice)
    add_test(NAME testCode COMMAND testCode)
endif()

# Check compiler flags
# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")