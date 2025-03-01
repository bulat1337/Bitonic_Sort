#pragma once

#include <format>
#include <iostream>

#include "log.h"

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

template <typename T> void hight_dump(const std::vector<T> &data)
{
	std::cout << std::format("{:#^{}}", "", 55) << '\n';
    for (const auto &elem : data)
    {
        std::cout << std::format("{:{}}", "", elem) << elem << '\n';
    }

	std::cout << std::format("{:#^{}}", "", 55) << '\n';
}

template <typename C>
void dump(const C& container) {
    using T = typename C::value_type; // Автоматическое определение типа элемента
    std::copy(std::begin(container), std::end(container),
              std::ostream_iterator<T>(std::clog, " "));
    std::clog << '\n';
}

}; // namespace utils

}; // namespace bitonic
