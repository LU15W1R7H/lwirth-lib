#pragma once

#include "Standard.hpp"

#include <cmath>
#include <utility>

namespace lw
{
	const f32 PI = 3.14159265359f;
	const f32 PI2 = 6.28318530718f;
	const f32 E = 2.71828182846f;

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

	constexpr inline f32 pow(f32 base, f32 exponent)
	{
		return std::pow(base, exponent);
	}

	constexpr inline f32 root(f32 value, f32 power)
	{
		return lw::pow(value, 1.f / power);
	}

	constexpr inline f32 sqrt(f32 value)
	{
		return std::sqrt(value);
	}

	template<typename T>
	constexpr T lerp(const T& start, const T& dest, f32 beta)
	{
		return static_cast<T>((1.f - beta) * start + beta * dest);
	}
}