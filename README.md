# Task Library Benchmark

A non-comprehensive benchmark of several task/fiber scheduling libraries. Includes OpenMP, cpp-task-flow, marl, and tbb. I made this because I was interested in task-based parallelism and wanted
to see how different libraries compare. I also thought it might be useful to have an independent repository where these benchmarks are setup. This repository is not meant to the `final` word
on anything related to these libraries.

## Note on benchmarks

Most of these benchmarks have been taken from [cpp-taskflow](https://github.com/cpp-taskflow/cpp-taskflow) without any significant modification.
The only difference is this repository uses [google benchmark](https://github.com/google/benchmark) to provide statistically significant results instead.
Plots are then generated using python from the saved output of `benchmark`.

## Install

1. `mkdir cmake-build && cd cmake-build` .
2. `cmake .. -DFETCHCONTENT_QUIET=OFF` . The flag `DFETCHCONTENT_QUIET=OFF` allows you to see progress of downloading the required dependencies.
3. `cmake --build .` 

## Run Benchmarks

1. `./cmake-build/bin/run-bench --benchmark_out=./results/tlb.json --benchmark_repetitions=3 --benchmark_report_aggregates_only` 
2. `python scripts/visualize_gbench.py` 

