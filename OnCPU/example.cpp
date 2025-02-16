#include <algorithm>
#include <iostream>
#include <numeric>
#include <stdexcept>
#include <vector>
#include <random>

#include "../utils/include/utils.h"

#ifndef CL_HPP_TARGET_OPENCL_VERSION
#define CL_HPP_MINIMUM_OPENCL_VERSION 120
#define CL_HPP_TARGET_OPENCL_VERSION 120
#endif

#define CL_HPP_CL_1_2_DEFAULT_BUILD
#define CL_HPP_ENABLE_EXCEPTIONS

#include "include/opencl.hpp"

#ifndef ANALYZE
#define ANALYZE 1
#endif

#define dbgs                                                                   \
    if (!ANALYZE)                                                              \
    {                                                                          \
    }                                                                          \
    else                                                                       \
        std::cout

constexpr size_t ARR_SIZE = 64;
constexpr size_t LOCAL_SIZE = 4;

#define STRINGIFY(...) #__VA_ARGS__

inline void swap(int* lhs, int* rhs)
{
	int tmp = *lhs;
	*lhs = *rhs;
	*rhs = tmp;
}

//-------------------------------- Kernel --------------------------------
const char *vakernel = STRINGIFY(__kernel void bitonic_sort(
    __global int *data_buf, int data_size) {

	// int global_id = get_global_id(0);
	// int local_id  = get_local_id(0);
	// int group_id = get_group_id(0);

    for (size_t block_size = 2; block_size <= data_size; block_size *= 2)
	{
        for (size_t lhs_id = 0; lhs_id < data_size; lhs_id += block_size)
		{
            bool is_ascending = ((lhs_id / block_size) % 2 == 0);

			for (size_t k = block_size / 2; k > 0; k /= 2)
				for (size_t i = lhs_id; i + k < lhs_id + block_size; ++i)
					if ((data_buf[i] > data_buf[i + k]) == is_ascending)
					{
                        int tmp = data_buf[i];
                        data_buf[i] = data_buf[i + k];
                        data_buf[i + k] = tmp;
					}
        }
    }
});
//-----------------------------------------------------------------------------

class OclApp
{
    cl::Platform platform_;
    cl::Context context_;
    cl::CommandQueue queue_;

    using bsort_t = cl::KernelFunctor<cl::Buffer, int>;

    static cl::Platform select_platform()
    {
        cl::vector<cl::Platform> platforms;
        cl::Platform::get(&platforms);

        for (auto p : platforms)
        {
            cl_uint ndevices = 0;
            ::clGetDeviceIDs(p(), CL_DEVICE_TYPE_GPU, 0, NULL, &ndevices);
            if (ndevices > 0)

                return cl::Platform(
                    p);
        }

        throw std::runtime_error("Не выбрано ни одной платформы");
    }

    static cl::Context get_gpu_context(cl_platform_id PId)
	{
		cl_context_properties properties[] = {
			CL_CONTEXT_PLATFORM,
			reinterpret_cast<cl_context_properties>(PId),
			0
		};

		return cl::Context(CL_DEVICE_TYPE_GPU, properties);
	}

  public:
    OclApp()
        : platform_(select_platform())
        , context_(get_gpu_context(platform_()))
    {
		auto devices = context_.getInfo<CL_CONTEXT_DEVICES>();
		cl::Device device = devices.front();
		queue_ = cl::CommandQueue(context_, device, CL_QUEUE_PROFILING_ENABLE);

        cl::string name = platform_.getInfo<CL_PLATFORM_NAME>();
        cl::string profile = platform_.getInfo<CL_PLATFORM_PROFILE>();
        dbgs << "Выбрано: " << name << ": " << profile << std::endl;
    }

    void bsort(cl_int* data, size_t data_size)
	{
		size_t nbytes = data_size * sizeof(cl_int);

		cl::Buffer data_buf(context_, CL_MEM_READ_WRITE, nbytes);

		cl::copy(queue_, data, data + data_size, data_buf);

		cl::Program program(context_, vakernel, true);

		bsort_t bitonic_sort(program, "bitonic_sort");

		cl::NDRange GlobalRange(data_size);
		cl::NDRange LocalRange(LOCAL_SIZE);

		cl::EnqueueArgs Args(queue_, GlobalRange, LocalRange);

		cl::Event evt = bitonic_sort(Args, data_buf, data_size);
		evt.wait();
		auto start_time = evt.getProfilingInfo<CL_PROFILING_COMMAND_START>();
		auto end_time = evt.getProfilingInfo<CL_PROFILING_COMMAND_END>();

		std::cout << "Duration: " << end_time - start_time << " nanosec\n";

		cl::copy(queue_, data_buf, data, data + data_size);
	}
};

int main()
try
{
    OclApp app;

	auto nelems = static_cast<size_t>(1) << 10;

    cl::vector<cl_int> data;
	data.reserve(nelems);

	std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 50);

    for (size_t id = 0; id < nelems; ++id) {
        data.push_back(dis(gen));
    }

	std::cout << "INITIAL:\n";
    bitonic::utils::hight_dump(data);

    app.bsort(data.data(), data.size());

	std::cout << "SORTED:\n";
    bitonic::utils::hight_dump(data);
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
