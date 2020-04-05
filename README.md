# Task Library Benchmark

A non-comprehensive benchmark of several task/fiber scheduling libraries. Includes OpenMP, cpp-task-flow, marl, and tbb.

## Install

1. `mkdir cmake-build && cd cmake-build` .
2. `cmake .. -DFETCHCONTENT_QUIET=OFF` . The flag `DFETCHCONTENT_QUIET=OFF` allows you to see progress of downloading the required dependencies.
3. `cmake --build .` 

