#include <benchmark/benchmark.h>
#include "Common.hpp"
#include "TaskFlow/tf_benches.hpp"

using namespace TaskLibraryBenchmark;

void BM_LinearSearch_TF(benchmark::State& state)
{
    int problem_size = 1 << state.range(0);
    int num_threads = state.range(1);
    for (auto _ : state) {
        auto counter = TF::linear_chain(problem_size, num_threads);
        benchmark::DoNotOptimize(counter+1);
    }
}

BENCHMARK(BM_LinearSearch_TF)->RangeMultiplier(2)->Ranges({{2, 24}, {1, 8}})->UseRealTime()->Unit(benchmark::kMicrosecond);