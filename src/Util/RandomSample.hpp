#ifndef RandomSample_H
#define RandomSample_H

#include <vector>
#include "LoDash/LoDash.hpp"

namespace Util
{
	static int RandomSample(std::vector<double> weights, const uint64_t randomNumber, uint32_t precision = 6)
	{
		auto _precision = [&]() constexpr -> uint64_t
			{
				uint64_t result = 1;
				for (int i = 0; i < precision; i++)
				{
					result *= 10;
				}

				return result;
			}
		();

		std::vector<uint64_t> intList = LoDash::Map(weights,
			[&](const double& data)
			{
				return static_cast<uint64_t>(data * _precision);
			}
		);

		uint64_t sum = LoDash::Sum(intList);
		uint64_t random = randomNumber % sum;
		uint64_t temp = 0;
		for (std::size_t i = 0; i < intList.size(); i++)
		{
			temp += intList[i];
			if (random < temp)
			{
				return i;
			}
		}

		return 0;
	}
}

#endif // !RandomSample_H