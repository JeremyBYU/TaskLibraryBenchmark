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

############# End Build Options #####################

find_package(Threads REQUIRED)
find_package(OpenMP REQUIRED)

add_executable(run-bench ${CMAKE_CURRENT_SOURCE_DIR}/run-bench.cpp 
                         ${CMAKE_CURRENT_SOURCE_DIR}/OpenMP/omp_benches.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/TaskFlow/tf_benches.cpp
                        )
target_include_directories(run-bench PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(run-bench PRIVATE benchmark Threads::Threads OpenMP::OpenMP_CXX)
target_link_libraries_system(run-bench Cpp-Taskflow)