cmake_minimum_required(VERSION 3.11)
# Project Details
set(PROJECT_NAME "TaskLibraryBenchmark")
set(PROJECT_NAME_SHORT "TLB")
set(PROJECT_VERSION 0.1.0)

# Set Project Properties
project(${PROJECT_NAME} VERSION ${PROJECT_VERSION}
DESCRIPTION "Benchmarks CPP Task Libraries"
LANGUAGES CXX)
# Set Global Properties
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

# Output Folders
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin)

# CMake Functions
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Extras.cmake)

# Build Options
# Options for TLB
option(TLB_BUILD_WERROR "TLB - Add Werror flag to build (turns warnings into errors)" OFF)

############# Start Build Options ###################

# Configure optimization
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O3 -DNDEBUG -march=native")
    message("-- Configuring release build")
endif()

# Set GLOBAL CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS}")

# Add any dependencies needed by our library
add_subdirectory("thirdparty")

add_subdirectory("src")

