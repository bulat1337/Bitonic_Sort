#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <format>

namespace bitonic
{

namespace utils
{

const double fp_tolerance = 1e-8;

inline int cmp_double(double lhs, double rhs)
{
    if (std::isnan(lhs) && std::isnan(rhs))
    {
        return 0;
    }
    if (fabs(lhs - rhs) < fp_tolerance)
    {
        return 0;
    }
    else if ((lhs - rhs) > fp_tolerance)
    {
        return 1;
    }
    else
    {
        return -1;
    }
}

template <typename T>
void hight_dump(const std::vector<T>& data)
{
	for (const auto& elem : data)
	{
		std::cout << std::format("{:{}}", "", elem) << elem << '\n';
	}
}

}; // namespace utils

}; // namespace matrix

#endif // UTILS_H
