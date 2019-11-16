cmake_minimum_required(VERSION 3.1)

project(SweepDecoder)

# SET(CMAKE_CXX_COMPILER /usr/bin/g++)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release")
endif()

# Turn on with 'cmake -Dtest=ON'
option(test "Build all tests." OFF)
# Turn on with 'cmake -Dprofile=ON'
option(profile "Profile using grpof")

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(test ON)
    # set(profile ON)
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")

set(SOURCE_FILES main.cpp)
add_executable(SweepDecoder ${SOURCE_FILES})

# Download and include pcg-cpp
include(ExternalProject)
ExternalProject_Add(
    pcg-cpp
    PREFIX ${CMAKE_BINARY_DIR}/pcg-cpp
    GIT_REPOSITORY https://github.com/imneme/pcg-cpp.git
    GIT_TAG master
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    INSTALL_COMMAND ""
    )
ExternalProject_Get_Property(pcg-cpp source_dir)
set(pcg_include_dir ${source_dir}/include)
include_directories(${pcg_include_dir})

# Include my library
include_directories(src)

# Link to my library
set(LIB_FILES src/lattice.h src/lattice.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicToricLattice.h src/rhombicToricLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicLattice.h src/rhombicLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/cubicToricLattice.h src/cubicToricLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/cubicLattice.h src/cubicLattice.cpp)
set(LIB_FILES ${LIB_FILES} src/code.h src/code.cpp)
set(LIB_FILES ${LIB_FILES} src/rhombicCode.h src/rhombicCode.cpp)
set(LIB_FILES ${LIB_FILES} src/cubicCode.h src/cubicCode.cpp)
set(LIB_FILES ${LIB_FILES} src/decoder.h)
add_library(SweepLib ${LIB_FILES}) 
add_dependencies(SweepLib pcg-cpp) # Important! Ensures that pcg downloaded before building library
target_link_libraries(SweepDecoder SweepLib)

if (test)
    enable_testing()
    add_definitions(-DSCTEST)
    include(CTest)
    
    # Add my tests
    add_subdirectory(tests)

    # Download and unpack googletest at configure time
    configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
    execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "CMake step for googletest failed: ${result}")
    endif()
    execute_process(COMMAND ${CMAKE_COMMAND} --build .
        RESULT_VARIABLE result
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
    if(result)
        message(FATAL_ERROR "Build step for googletest failed: ${result}")
    endif()

    # Prevent overriding the parent project's compiler/linker
    # settings on Windows
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    # Add googletest directly to our build. This defines
    # the gtest and gtest_main targets.
    add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                    ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                    EXCLUDE_FROM_ALL)

    # The gtest/gtest_main targets carry header search path
    # dependencies automatically when using CMake 2.8.11 or
    # later. Otherwise we have to add them here ourselves.
    if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
    endif()

    # Add test executables
    add_executable(testLattice tests/test_lattice.cpp)
    add_executable(testRhombicToricLattice tests/test_rhombicToricLattice.cpp)
    add_executable(testRhombicLattice tests/test_rhombicLattice.cpp)
    add_executable(testCubicLattice tests/test_cubicLattice.cpp)
    add_executable(testRhombicCodeToric tests/test_rhombicCode_toric.cpp)
    add_executable(testCubicCodeToric tests/test_cubicCode_toric.cpp)
    add_executable(testRhombicCodeBoundaries tests/test_rhombicCode_boundaries.cpp)
    add_executable(testCubicCodeBoundaries tests/test_cubicCode_boundaries.cpp)

    # Standard googletest linking
    target_link_libraries(testLattice gtest gtest_main)
    target_link_libraries(testRhombicToricLattice gtest gtest_main)
    target_link_libraries(testRhombicLattice gtest gtest_main)
    target_link_libraries(testCubicLattice gtest gtest_main)
    target_link_libraries(testRhombicCodeToric gtest gtest_main)
    target_link_libraries(testRhombicCodeBoundaries gtest gtest_main)
    target_link_libraries(testCubicCodeBoundaries gtest gtest_main)
    target_link_libraries(testCubicCodeToric gtest gtest_main)

    # Link to my library
    target_link_libraries(testLattice SweepLib)
    target_link_libraries(testRhombicToricLattice SweepLib)
    target_link_libraries(testRhombicLattice SweepLib)
    target_link_libraries(testCubicLattice SweepLib)
    target_link_libraries(testRhombicCodeToric SweepLib)
    target_link_libraries(testRhombicCodeBoundaries SweepLib)
    target_link_libraries(testCubicCodeBoundaries SweepLib)
    target_link_libraries(testCubicCodeToric SweepLib)

    # Enable running tests with 'make test'
    add_test(NAME testLattice COMMAND testLattice)
    add_test(NAME testRhombicToricLattice COMMAND testRhombicToricLattice)
    add_test(NAME testRhombicLattice COMMAND testRhombicLattice)
    add_test(NAME testCubicLattice COMMAND testCubicLattice)
    add_test(NAME testRhombicCodeToric COMMAND testRhombicCodeToric)
    add_test(NAME testRhombicCodeBoundaries COMMAND testRhombicCodeBoundaries)
    add_test(NAME testCubicCodeBoundaries COMMAND testCubicCodeBoundaries)
    add_test(NAME testCubicCodeToric COMMAND testCubicCodeToric)
endif()

if (profile)
    # Set gprof flags
    SET(GCC_PROFILE_COMPILE_FLAGS "-pg")
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GCC_PROFILE_COMPILE_FLAGS}")
    SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -pg")
    SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -pg")
endif()