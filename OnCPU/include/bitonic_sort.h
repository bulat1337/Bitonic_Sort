#pragma once

#include <vector>
#include <algorithm>
#include <iostream>
#include <stack>

namespace bitonic
{
	template <typename RandomIt>
	void Sort(RandomIt begin, RandomIt end)
	{
		using range_t = std::pair<RandomIt, RandomIt>;
		std::stack<range_t> stk;
		stk.push({begin, end});

		while (!stk.empty())
		{
			auto [cur_begin, cur_end] = stk.top();
			stk.pop();

			size_t data_size = std::distance(cur_begin, cur_end);

			if (data_size <= 1) continue;

			auto mid = std::next(cur_begin, data_size / 2);

			auto lhs_iter = cur_begin;
			auto rhs_iter = mid;

			for (;rhs_iter != cur_end; ++lhs_iter, ++rhs_iter)
			{
				if (*lhs_iter > *rhs_iter)
				{
					std::cout << "swapping " << *lhs_iter << " and " << *rhs_iter << '\n';
					std::iter_swap(lhs_iter, rhs_iter);
				}
			}

			stk.push({cur_begin, mid});
			stk.push({mid, cur_end});
		}
	}


};
