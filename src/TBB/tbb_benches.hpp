#ifndef TLB_TBB
#define TLB_TBB

#include "Common.hpp"

#include <tbb/task_scheduler_init.h>
#include <tbb/flow_graph.h>

namespace TaskLibraryBenchmark {

namespace TBB {

size_t linear_chain(size_t length, unsigned num_threads)
{

    using namespace tbb;
    using namespace tbb::flow;

    size_t counter = 0;

    tbb::task_scheduler_init init(num_threads);

    graph g;

    std::vector<continue_node<continue_msg>*> tasks(length);

    for (size_t i = 0; i < tasks.size(); i++)
    {
        tasks[i] = new continue_node<continue_msg>(g, [&](const continue_msg&) { counter++; });
        if (i)
        {
            make_edge(*tasks[i - 1], *tasks[i]);
        }
    }

    tasks[0]->try_put(continue_msg());
    g.wait_for_all();

    for (auto& task : tasks)
    {
        delete task;
    }

    assert(counter == tasks.size());
}

// binary_tree_tbb
size_t binary_tree(size_t num_layers, unsigned num_threads)
{

    using namespace tbb;
    using namespace tbb::flow;

    tbb::task_scheduler_init init(num_threads);

    std::atomic<size_t> counter{0};

    graph g;

    std::vector<continue_node<continue_msg>*> tasks(1 << num_layers);

    for (unsigned i = 1; i < tasks.size(); i++)
    {
        tasks[i] = new continue_node<continue_msg>(
            g, [&](const continue_msg&) { counter.fetch_add(1, std::memory_order_relaxed); });
    }

    for (unsigned i = 1; i < tasks.size(); i++)
    {
        unsigned l = i << 1;
        unsigned r = l + 1;
        if (l < tasks.size() && r < tasks.size())
        {
            make_edge(*tasks[i], *tasks[l]);
            make_edge(*tasks[i], *tasks[r]);
        }
    }

    tasks[1]->try_put(continue_msg());
    g.wait_for_all();

    for (auto& task : tasks)
    {
        delete task;
    }

    assert(counter + 1 == tasks.size());
    return counter;
}

} // namespace TBB
} // namespace TaskLibraryBenchmark

#endif
