#include <algorithm>
#include <format>
#include <iostream>
#include <vector>
#include <random>
#include <fstream>
#include <chrono>

#include "bitonic_sort.h"
#include "utils.h"

int main()
{
	auto nelems = static_cast<size_t>(1) << 15;

    std::vector<int> data;
	data.reserve(nelems);

	std::string file_name("../unit_tests/data/common/basic_data_32768.dat");

	std::ifstream input(file_name);

	if (!input.is_open())
	{
		std::cerr << "Can't open " << file_name << '\n';
		return 0;
	}

    for (size_t id = 0; id < nelems; ++id)
	{
		int elem = 0;
		input >> elem;
        data.push_back(elem);
    }

	dbgs << "INITIAL:\n";
    bitonic::utils::hight_dump(data);

    auto start = std::chrono::high_resolution_clock::now();
	bitonic::Sort(data);
	auto end = std::chrono::high_resolution_clock::now();

	auto duration = std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();

	dbgs << "RESULT:\n";
    bitonic::utils::hight_dump(data);

	std::cout << "Duration: " << duration << " nanosec\n";

    return 0;
}
