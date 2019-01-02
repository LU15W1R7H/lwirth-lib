#ifndef LWIRTH_UTILS_HPP
#define LWIRTH_UTILS_HPP

#include "../Standard.hpp"

#include <math.h>
#include <utility>
#include <limits>
#include <chrono>

#define precCast(x) static_cast<f64>(x)
#define LW_FORWARD(var) std::forward<decltype(var)>(var)


#define DISABLE_COPYING(T)\
	T(const T&) = delete; \
	T& operator=(const T&) = delete;

#define DISABLE_MOVING(T)\
	T(T&&) = delete; \
	T& operator=(T&&) = delete;

#define DISABLE_COP_MOV(T)\
	DISABLE_COPYING(T); \
	DISABLE_MOVING(T);

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
		double raised = pow(10.f, static_cast<f32>(precision));
		double temp = value * raised;
		T rounded = static_cast<T>(floor(temp));

		if (static_cast<T>(temp) - rounded >= static_cast<T>(0.5)) {
			rounded = static_cast<T>(ceil(temp));
		}

		return static_cast<U>(rounded / raised);
	}


	template<typename T, typename U>
	constexpr inline U map(T value, T oldMin, T oldMax, U newMin, U newMax)
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
		func(LW_FORWARD(args)...);
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
	
    template<typename ForwardIter, typename ElementCallable, typename ... Args>
    void fill(ForwardIter first, ForwardIter last, ElementCallable callable, Args &&... args)
    {
        for(; first != last; ++first)
        {
            *first = callable(LW_FORWARDS(args)...);
        }
    }
   


}

#endif //LWIRTH_UTILS_HPP