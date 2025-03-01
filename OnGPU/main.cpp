#include "bitonic_sort.h"

#include <iostream>
#include <vector>
#include <random>

int main()
try
{
	bitonic::OclApp app;

	std::vector<int> vec{4, 2, 7, 8, 1, 4, 6, 7, 43, 14, 32, 5, 75, 23, 6, 76};

	std::cout << "INTITIAL:\n";
	bitonic::utils::dump(vec);

	app.bsort(vec, true);

	std::cout << "SORTED:\n";
	bitonic::utils::dump(vec);
}
catch (cl::Error &err)
{
    std::cerr << "OpenCL error: " << err.err() << ":" << err.what()
              << std::endl;
    return -1;
}
catch (std::runtime_error &err)
{
    std::cerr << "runtime error: " << err.what() << std::endl;
    return -1;
}
catch (...)
{
    std::cerr << "unknown error\n";
    return -1;
}
