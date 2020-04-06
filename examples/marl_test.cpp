#include "Marl/marl_benches.hpp"

using namespace TaskLibraryBenchmark;

int main(int argc, char const *argv[])
{
    size_t num_threads = 1;
    size_t problem_size = 16;
    std::cout << "before" << std::endl;
    auto result = Marl::binary_tree(problem_size, num_threads);
    std::cout << "Result is: " << result << std::endl;
    
    return 0;
}
