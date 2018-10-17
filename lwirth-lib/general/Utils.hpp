#ifndef LWIRTH_UTILS_HPP
#define LWIRTH_UTILS_HPP

#include "../Standard.hpp"

#include <math.h>
#include <utility>
#include <limits>
#include <chrono>

#define precCast(x) static_cast<f64>(x)
#define LW_FORWARD(var) std::forward<decltype(var)>(var)

namespace lw
{
	
	void sleep(size_t milliseconds);


	//template functions

    template<typename ... Pack>
    void swallow(Pack &&...)
    {}

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

	template<typename Func, typename ... Args>
	i64 chronometer(Func func, Args &&... args)
	{
		const auto start = std::chrono::high_resolution_clock::now();
		func(args...);
		return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count();
	}

	//strong types

	template<typename Type, typename Tag>
	class StrongType
    {
    private:
		   Type val;
    public:
        explicit StrongType() : val{} {};
        explicit StrongType(Type newVal) : val(newVal) {}
        Type get() const noexcept {return val;}
        void set(Type newVal) noexcept {val = newVal;}
    };

    using ExampleStrongType = StrongType<u32, struct unique>;

    //add ArithmeticStrongType


}

#endif //LWIRTH_UTILS_HPP