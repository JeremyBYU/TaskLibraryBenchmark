#ifndef TLB_MARL
#define TLB_MARL

#include "Common.hpp"

#include "marl/defer.h"
#include "marl/event.h"
#include "marl/scheduler.h"
#include "marl/waitgroup.h"

namespace TaskLibraryBenchmark {

namespace Marl {

size_t linear_chain(size_t length, unsigned num_threads)
{

    // Setup the marl environment
    marl::Scheduler scheduler;
    scheduler.bind();
    scheduler.setWorkerThreadCount(num_threads);
    defer(scheduler.unbind()); // Automatically unbind before returning.

    size_t counter = 0;
    std::vector<marl::Event> events;
    events.resize(length);

    events.front().signal();

    for (size_t i = 0; i < length - 1; ++i)
    {
        auto my_event = events[i];
        auto next_event = events[i + 1];
        marl::schedule(marl::Task(
            [=, &counter] {
                defer(next_event.signal());
                my_event.wait();
                counter++;
            },
            marl::Task::Flags::SameThread));
    }

    events.back().wait();; // wait until all tasks are completed

    return counter;
}

size_t binary_tree(size_t num_layers, unsigned num_threads)
{
    std::atomic<size_t> counter{0};

    marl::Scheduler scheduler;
    scheduler.bind();
    scheduler.setWorkerThreadCount(num_threads);
    defer(scheduler.unbind()); // Automatically unbind before returning.

    unsigned total_tasks = 1 << num_layers;
    std::vector<marl::WaitGroup> all_wgs;
    all_wgs.resize(total_tasks);

    for (unsigned i = 0; i < total_tasks; i++)
    {
        unsigned l = i << 1;
        unsigned r = l + 1;
        if (l < all_wgs.size() && r < all_wgs.size())
        {
            all_wgs[i].add(2);
        }
    }

    for (unsigned i = total_tasks - 1; i > 0; i--)
    {
        auto my_wg = all_wgs[i];
        auto parent_wg = all_wgs[i >> 1];
        marl::schedule([=, &counter] {
            defer(parent_wg.done());
            my_wg.wait();
            counter.fetch_add(1, std::memory_order_relaxed);
        });
    }

    all_wgs[1].wait();

    return counter;
}


} // namespace Marl
} // namespace TaskLibraryBenchmark

#endif
