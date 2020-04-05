############# Start Build Options ###################

# Configure optimization
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OPTIMIZATION_FLAGS "-O0 -DDEBUG")
    message("-- Configuring debug build")
else()
    set(OPTIMIZATION_FLAGS "-O3 -DNDEBUG -march=native")
    message("-- Configuring release build")
endif()

# # Enable warnings, will set to compile with src/CMakeLists.txt
# set(DESIRED_WARNINGS "-Wall -Wextra -Wconversion -Wunreachable-code -Wuninitialized -pedantic-errors -Wold-style-cast -Wno-error=unused-variable -Wshadow -Wfloat-equal -Weffc++")
# set(MINIMAL_DESIRED_WARNINGS "-Wall -Wextra -pedantic-errors")
# if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
#     set(DESIRED_WARNINGS "${DESIRED_WARNINGS} -Wmost")
# endif()
# separate_arguments(DESIRED_WARNINGS)
# separate_arguments(MINIMAL_DESIRED_WARNINGS)

# if (OPF_BUILD_WERROR)
#     set(DESIRED_WARNINGS "${DESIRED_WARNINGS} -Werror")
#     set(MINIMAL_DESIRED_WARNINGS "${MINIMAL_DESIRED_WARNINGS} -Werror")
# endif()

# Set GLOBAL CMAKE_CXX_FLAGS
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPTIMIZATION_FLAGS}")

############# End Build Options #####################

find_package(Threads REQUIRED)

add_executable(run-bench ${CMAKE_CURRENT_SOURCE_DIR}/run-bench.cpp 
                         ${CMAKE_CURRENT_SOURCE_DIR}/omp_benches.cpp
                         ${CMAKE_CURRENT_SOURCE_DIR}/tf_benches.cpp
                        )
target_include_directories(run-bench PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(run-bench PRIVATE benchmark Threads::Threads)
target_link_libraries_system(run-bench Cpp-Taskflow)