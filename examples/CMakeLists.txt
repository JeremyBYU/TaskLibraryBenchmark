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

add_executable(example-marl ${CMAKE_CURRENT_SOURCE_DIR}/marl_test.cpp 

                        )
target_include_directories(example-marl PRIVATE "${PROJECT_SOURCE_DIR}/src")

target_link_libraries(example-marl PRIVATE benchmark Threads::Threads OpenMP::OpenMP_CXX)
target_link_libraries_system(example-marl Cpp-Taskflow tbb marl)