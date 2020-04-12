#include <benchmark/benchmark.h>
#include "Common.hpp"
#include "Marl/marl_benches.hpp"

using namespace TaskLibraryBenchmark;

void BM_LinearChain_Marl(benchmark::State& state)
{
    int problem_size = 1 << state.range(0);
    int num_threads = state.range(1);
    for (auto _ : state)
    {
        auto counter = Marl::linear_chain(problem_size, num_threads);
        benchmark::DoNotOptimize(counter + 1);
    }
}

void BM_BinaryTree_Marl(benchmark::State& state)
{
    int problem_size = state.range(0);
    int num_threads = state.range(1);
    for (auto _ : state)
    {
        auto counter = Marl::binary_tree(problem_size, num_threads);
        benchmark::DoNotOptimize(counter + 1);
    }
}

BENCHMARK(BM_LinearChain_Marl)
    ->Apply(CustomArguments_LC)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);
BENCHMARK(BM_BinaryTree_Marl)
    ->Apply(CustomArguments_BT)
    ->UseRealTime()
    ->Unit(benchmark::kMillisecond);