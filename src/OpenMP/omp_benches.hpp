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

    // assert(counter == length);

    delete[] D;
    return counter;
}

// binary_tree_omp
size_t binary_tree(size_t num_layers, unsigned num_threads) {

  std::atomic<size_t> counter {0};
  
  size_t N = 1 << num_layers;
  size_t *D = new size_t [N]; 

  #pragma omp parallel num_threads(num_threads)
  {
    #pragma omp single
    {
      for(size_t i = 1; i<N; ++i) {
        
        size_t p = i / 2;
        size_t l = i * 2;
        size_t r = l + 1;
        
        if(l < N && r < N) {
          #pragma omp task firstprivate(i) depend(out:D[l], D[r]) depend(in:D[p])
          {
            //printf("%d\n", i);
            counter.fetch_add(1, std::memory_order_relaxed);
          }
        }
        else {
          #pragma omp task firstprivate(i) depend(in:D[p])
          {
            //printf("%d\n", i);
            counter.fetch_add(1, std::memory_order_relaxed);
          }
        }
      }
    }
  }

  assert((counter + 1) == N);

  delete [] D;

  return counter;
}

} // namespace OMP
} // namespace TaskLibraryBenchmark

#endif