#include "bitonic_sort.h"

#include <iostream>
#include <vector>
#include <random>
#include <ranges>
#include <algorithm>
#include <chrono>

int main()
try
{
	size_t N{};

	std::cin >> N;

	if (bitonic::utils::cinFailed()) return 1;

	std::vector<int> vec;
	vec.reserve(N);

	for ([[maybe_unused]]auto _ : std::views::iota(size_t(0), N))
	{
		int elem{};

		std::cin >> elem;
		if (bitonic::utils::cinFailed()) return 1;

		vec.push_back(elem);
	}

	#ifdef ENABLE_LOGGING
	std::cout << "Initial data:\n";
	bitonic::utils::dump(vec);
	#endif

	#ifdef SORT_ON_CPU
	std::vector<int> vec_copy(vec);

	auto start = std::chrono::high_resolution_clock::now();
	std::sort(vec_copy.begin(), vec_copy.end());
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = end - start;

	LOG("CPU Duration: {} nanosec\n", duration.count());

	#endif

	bitonic::OclApp app;

	app.bsort(vec, /* incr_order = */ true);

	MSG("Sorted data:\n");

	bitonic::utils::dump(vec);

	return 0;
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
