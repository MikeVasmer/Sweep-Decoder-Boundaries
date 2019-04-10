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

# Turn on with 'cmake -Dprofile=ON'
option(profile "Profile using grpof")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp)
add_executable(RhombicSweep ${SOURCE_FILES})

# Include my files and boost
include_directories(src)
# find_package(Boost REQUIRED)
# include_directories(${Boost_INCLUDE_DIRS})
include_directories(include/pcg-cpp-0.98/include)

# Link to my library
set(LIB_FILES src/lattice.h src/lattice.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicToricLattice.h src/rhombicToricLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicLattice.h src/rhombicLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/cubicLattice.h src/cubicLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/code.h src/code.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicCode.h src/rhombicCode.cpp)
set(LIB_FILES ${LIB_FILES} src/decoder.h)
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
    add_executable(testRhombicToricLattice tests/test_rhombicToricLattice.cpp)
    add_executable(testRhombicLattice tests/test_rhombicLattice.cpp)
    add_executable(testCubicLattice tests/test_cubicLattice.cpp)
    add_executable(testRhombicCodeToric tests/test_rhombicCode_toric.cpp)
    add_executable(testRhombicCodeBoundaries tests/test_rhombicCode_boundaries.cpp)
    # Standard googletest linking
    target_link_libraries(testLattice gtest gtest_main)
    target_link_libraries(testRhombicToricLattice gtest gtest_main)
    target_link_libraries(testRhombicLattice gtest gtest_main)
    target_link_libraries(testCubicLattice gtest gtest_main)
    target_link_libraries(testRhombicCodeToric gtest gtest_main)
    target_link_libraries(testRhombicCodeBoundaries gtest gtest_main)
    # Link to my library
    target_link_libraries(testLattice SweepLib)
    target_link_libraries(testRhombicToricLattice SweepLib)
    target_link_libraries(testRhombicLattice SweepLib)
    target_link_libraries(testCubicLattice SweepLib)
    target_link_libraries(testRhombicCodeToric SweepLib)
    target_link_libraries(testRhombicCodeBoundaries SweepLib)
    # Enable running tests with 'make test'
    add_test(NAME testLattice COMMAND testLattice)
    add_test(NAME testRhombicToricLattice COMMAND testRhombicToricLattice)
    add_test(NAME testRhombicLattice COMMAND testRhombicLattice)
    add_test(NAME testCubicLattice COMMAND testCubicLattice)
    add_test(NAME testRhombicCodeToric COMMAND testRhombicCodeToric)
    add_test(NAME testRhombicCodeBoundaries COMMAND testRhombicCodeBoundaries)
endif()

if (profile)
    SET(GCC_PROFILE_COMPILE_FLAGS "-pg")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_PROFILE_COMPILE_FLAGS}")
endif()

# Check compiler flags
# message("CMAKE_CXX_FLAGS_DEBUG is ${CMAKE_CXX_FLAGS_DEBUG}")
# message("CMAKE_CXX_FLAGS_RELEASE is ${CMAKE_CXX_FLAGS_RELEASE}")