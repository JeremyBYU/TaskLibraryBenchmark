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
    marl::WaitGroup wg_counter(length);

    std::vector<marl::WaitGroup> wg_all;
    wg_all.reserve(length);

    for (size_t i = 0; i < length; ++i)
    {
        // create a wait task
        wg_all.emplace_back(1);
        if (i == 0)
        {
            marl::schedule([&wg_all, &counter, i, wg_counter] {
                auto my_wg = wg_all[i]; // copy by value, recommended
                defer(my_wg.done());
                defer(wg_counter.done());
                counter++;
            });
        }
        else
        {
            marl::schedule([&wg_all, &counter, i, wg_counter] {
                auto prior_wg = wg_all[i - 1]; // copy by value, recommended
                auto my_wg = wg_all[i];        // copy by value, recommended
                defer(my_wg.done());
                defer(wg_counter.done());
                prior_wg.wait();
                counter++;
            });
        }
    }

    wg_counter.wait(); // wait until all tasks are completed

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
    std::vector<marl::WaitGroup> wg_all;
    wg_all.reserve(total_tasks);
    for (int i = 0; i < total_tasks; ++i)
    {
        wg_all.emplace_back(1);
    }

    for (unsigned i = total_tasks - 1; i > 0; i--)
    {
        unsigned l = i << 1;
        unsigned r = l + 1;
        if (l < total_tasks && r < total_tasks)
        {
            marl::schedule([&wg_all, &counter, l, r, i] {
                auto my_wg = wg_all[i];
                defer(my_wg.done());

                auto wg_l = wg_all[l];
                auto wg_r = wg_all[r];
                // std::cout << "Task " << i << "; Waiting; depended on " << l << " and " << r << std::endl;
                wg_l.wait();
                wg_r.wait();

                // std::cout << "Task " << i << "; Done; depended on " << l << " and " << r << std::endl;
                counter.fetch_add(1, std::memory_order_relaxed);
            });
        }
        else
        {
            marl::schedule([&wg_all, &counter, l, r, i] {
                auto my_wg = wg_all[i];
                defer(my_wg.done());
                // std::cout << "Task " << i << "; Done;" << std::endl;
                counter.fetch_add(1, std::memory_order_relaxed);
            });
        }
    }

    auto top_task_wg = wg_all[1];
    top_task_wg.wait();

    size_t final_counter = counter;
    return final_counter;
}


} // namespace Marl
} // namespace TaskLibraryBenchmark

#endif
