#ifndef TLB_TF
#define TLB_TF

#include <taskflow/taskflow.hpp>

#include "omp.h"
#include "Common.hpp"

namespace TaskLibraryBenchmark {

namespace TF {

// binary_tree_taskflow
size_t linear_chain(size_t length, unsigned num_threads)
{

    size_t counter{0};

    std::vector<tf::Task> tasks(length);

    tf::Executor executor(num_threads);
    tf::Taskflow taskflow;

    for (size_t i = 0; i < length; ++i)
    {
        tasks[i] = taskflow.emplace([&]() { counter++; });
    }

    taskflow.linearize(tasks);

    executor.run(taskflow).get();
    return counter;
}

} // namespace TF
} // namespace TaskLibraryBenchmark

#endif
