#pragma once

#include "../Standard.hpp"

#include <math.h>
#include <utility>
#include <limits>

#define precCast(x) static_cast<f64>(x)
#define LW_FORWARD(var) std::forward<decltype(var)>(var)

namespace lw
{
	
	void sleep(size_t milliseconds);


	//template functions

	template<typename T, typename U>
	inline U roundCast(T value)
	{
		int precision = std::numeric_limits<U>::max_digits10;
		double raised = pow(10, precision);
		double temp = value * raised;
		T rounded = static_cast<T>(floor(temp));

		if (static_cast<T>(temp) - rounded >= static_cast<T>(0.5)) {
			rounded = static_cast<T>(ceil(temp));
		}

		return static_cast<U>(rounded / raised);
	}


	template<typename T, typename U>
	inline U map(T value, T oldMin, T oldMax, U newMin, U newMax)
	{
		f64 percise = (((precCast(value) - precCast(oldMin)) *
			(precCast(newMax) - precCast(newMin))) /
			(precCast(oldMax) - precCast(oldMin))) + precCast(newMin);

		U result = roundCast<double, U>(percise);

		return result;
	}


}
