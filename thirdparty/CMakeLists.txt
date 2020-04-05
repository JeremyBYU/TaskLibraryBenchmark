include(FetchContent)
# Put any LIBRARY dependencies in here


SET(BENCHMARK_ENABLE_TESTING OFF CACHE BOOL "Enable testing of the benchmark library.")
SET(TBB_BUILD_TESTS OFF CACHE BOOL "Build TBB tests and enable testing infrastructure")

find_package(OpenMP REQUIRED)
# target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)

FetchContent_Declare(
  extern_googlebenchmark
  GIT_REPOSITORY https://github.com/google/benchmark.git
  GIT_TAG        v1.5.0
  GIT_PROGRESS   TRUE
)
  
FetchContent_Declare(
  extern_cpptaskflow
  GIT_REPOSITORY https://github.com/cpp-taskflow/cpp-taskflow.git
  GIT_TAG        v2.4.0
  GIT_PROGRESS   TRUE
)

FetchContent_Declare(
  extern_marl
  GIT_REPOSITORY https://github.com/google/marl.git
  GIT_TAG        origin/master
  GIT_PROGRESS   TRUE
)

FetchContent_Declare(
  extern_tbb
  GIT_REPOSITORY https://github.com/wjakob/tbb.git
  GIT_TAG        origin/master
  GIT_PROGRESS   TRUE
)

# After the following call, the library will be installed
FetchContent_MakeAvailable(extern_cpptaskflow extern_marl extern_tbb extern_googlebenchmark)