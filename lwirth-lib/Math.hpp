#pragma once

#include "Standard.hpp"

#include <cmath>
#include <utility>

namespace lw
{
	const F32 PI = 3.14159265359f;
	const F32 PI2 = 6.28318530718f;
	const F32 E = 2.71828182846f;

	template<class T, class U>
	constexpr auto min(T const& t, U const& u)
	{
		return t < u ? t : u;
	}

	template<class T, class U>
	constexpr auto max(T const& t, U const& u)
	{
		return t > u ? t : u;
	}

	template<typename T>
	constexpr T add(T const& arg1, T const& arg2)
	{
		return arg1 + arg2;
	}

	template<typename T>
	constexpr T sub(T const& arg1, T const& arg2)
	{
		return arg1 - arg2;
	}

	template<typename T>
	constexpr T mul(T const& arg1, T const& arg2)
	{
		return arg1 * arg2;
	}

	template<typename T>
	constexpr T div(T const& arg1, T const& arg2)
	{
		return arg1 / arg2;
	}

	template<typename T>
	constexpr T mod(T const& arg1, T const& arg2)
	{
		return arg1 % arg2;
	}

	inline F32 pow(F32 base, F32 exponent)
	{
		return std::pow(base, exponent);
	}

	inline F32 root(F32 value, F32 power)
	{
		return lw::pow(value, 1.f / power);
	}

	inline F32 sqrt(F32 value)
	{
		return std::sqrt(value);
	}

	template<typename T>
	constexpr T lerp(const T& start, const T& dest, F32 beta)
	{
		return static_cast<T>((1.f - beta) * start + beta * dest);
	}
}