#include <benchmark/benchmark.h>
#include "Common.hpp"
#include "OpenMP/omp_benches.hpp"

using namespace TaskLibraryBenchmark;

void BM_LinearSearch_OMP(benchmark::State& state)
{
    int problem_size = 1 << state.range(0);
    int num_threads = state.range(1);
    // std::cout<< "Problem size: " <<  problem_size << std::endl;
    // std::cout<< "Number of threads: " <<  num_threads << std::endl;
    for (auto _ : state) {
        auto counter = OMP::linear_chain(problem_size, num_threads);
        benchmark::DoNotOptimize(counter+1);
        
    }
}

BENCHMARK(BM_LinearSearch_OMP)->RangeMultiplier(2)->Ranges({{2, 32}, {1, 8}})->UseRealTime()->Unit(benchmark::kMicrosecond);