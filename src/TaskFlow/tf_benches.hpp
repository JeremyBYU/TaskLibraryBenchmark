#ifndef TLB_TF
#define TLB_TF

#include "Common.hpp"
#include <taskflow/taskflow.hpp> 

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

// binary_tree_taskflow
size_t binary_tree(size_t num_layers, unsigned num_threads) {

  std::atomic<size_t> counter {0};

  std::vector<tf::Task> tasks(1 << num_layers);
  
  tf::Executor executor(num_threads);
  tf::Taskflow taskflow;

  for(unsigned i=1; i<tasks.size(); i++) {
    tasks[i] = taskflow.emplace([&](){
      counter.fetch_add(1, std::memory_order_relaxed);
    });
  }

  for(unsigned i=1; i<tasks.size(); i++) {
    unsigned l = i << 1;
    unsigned r = l + 1;
    if(l < tasks.size() && r < tasks.size()) {
      tasks[i].precede(tasks[l], tasks[r]);
    }
  }
  
  executor.run(taskflow).get();
  assert(counter + 1 == tasks.size());
  return counter;
}

} // namespace TF
} // namespace TaskLibraryBenchmark

#endif
