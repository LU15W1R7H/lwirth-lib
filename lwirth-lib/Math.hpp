#pragma once

#include "Standard.hpp"

#include <cmath>

namespace lw
{
	const F32 PI = 3.14159265359f;
	const F32 PI2 = 6.28318530718f;
	const F32 E = 2.71828182846f;

	template<typename T>
	API constexpr inline T min(T const& arg1, T const& arg2)
	{
		return arg1 < arg2 ? arg1 : arg2;
	}

	template<typename T>
	API constexpr inline T max(T const& arg1, T const& arg2)
	{
		return arg1 > arg2 ? arg1 : arg2;
	}

	template<typename T>
	API constexpr inline T add(T const& arg1, T const& arg2)
	{
		return arg1 + arg2;
	}

	template<typename T>
	API constexpr inline T sub(T const& arg1, T const& arg2)
	{
		return arg1 - arg2;
	}

	template<typename T>
	API constexpr inline T mul(T const& arg1, T const& arg2)
	{
		return arg1 * arg2;
	}

	template<typename T>
	API constexpr inline T div(T const& arg1, T const& arg2)
	{
		return arg1 / arg2;
	}

	template<typename T>
	API constexpr inline T mod(T const& arg1, T const& arg2)
	{
		return arg1 % arg2;
	}

	API inline F32 pow(F32 base, F32 exponent)
	{
		return std::pow(base, exponent);
	}

	API inline F32 root(F32 value, F32 power)
	{
		return lw::pow(value, 1.f / power);
	}

	API inline F32 sqrt(F32 value)
	{
		return std::sqrt(value);
	}

	template<typename T>
	API constexpr inline T lerp(const T& start, const T& dest, F32 beta)
	{
		return static_cast<T>((1.f - beta) * start + beta * dest);
	}
}