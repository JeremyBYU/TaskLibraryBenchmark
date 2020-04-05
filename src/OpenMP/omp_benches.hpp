#ifndef TLB_OMP
#define TLB_OMP

#include "omp.h"
#include "Common.hpp"

namespace TaskLibraryBenchmark {

namespace OMP {
// linear_chain_omp
size_t linear_chain(size_t length, unsigned num_threads)
{

    size_t counter = 0;
    size_t* D = new size_t[length];

#pragma omp parallel num_threads(num_threads)
    {
#pragma omp single
        {
            for (size_t i = 0; i < length; ++i)
            {

                if (i == 0)
                {
#pragma omp task firstprivate(i) depend(out : D[i])
                    {
                        ++counter;
                    }
                }
                else
                {
#pragma omp task firstprivate(i) depend(out : D[i]) depend(in : D[i - 1])
                    {
                        ++counter;
                    }
                }
            }
        }
    }

    assert(counter == length);

    delete[] D;
    return counter;
}

} // namespace OMP
} // namespace TaskLibraryBenchmark

#endif