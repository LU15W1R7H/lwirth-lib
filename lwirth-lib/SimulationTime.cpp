#include "stdafx.hpp"
#include "SimulationTime.hpp"

namespace lw
{
	namespace phy
	{
		Clock::Clock()
			: time(Time(0)), tickDuration(Duration(1))
		{
		}

		Clock::Clock(Time time, Duration tickDuration)
			: time(time), tickDuration(tickDuration)
		{
		}

		Duration Clock::tick()
		{
			time += tickDuration;
			return tickDuration;
		}

		Duration Clock::tick(Duration tickDuration)
		{
			time += tickDuration;
			return tickDuration;
		}

		
		Time::Time()
			: value(0)
		{
		}

		Time::Time(F32 value)
			: value(value)
		{
		}

		void Time::add(const Duration& d)
		{
			value += d.span;
		}

		void Time::sub(const Duration& d)
		{
			value -= d.span;
		}

		Time& Time::operator+=(const Duration & d)
		{
			add(d);
			return *this;
		}

		Time& Time::operator-=(const Duration & d)
		{
			sub(d);
			return *this;
		}

		Duration::Duration()
			: span(0)
		{
		}

		Duration::Duration(F32 span)
			: span(span)
		{
		}

		void Duration::add(const Duration& d)
		{
			span += d.span;
		}

		void Duration::sub(const Duration& d)
		{
			span -= d.span;
		}

		void Duration::mul(F32 scalar)
		{
			span *= scalar;
		}

		void Duration::div(F32 divisor)
		{
			span /= divisor;
		}

		Duration& Duration::operator+=(const Duration& d)
		{
			add(d);
			return *this;
		}

		Duration& Duration::operator-=(const Duration& d)
		{
			sub(d);
			return *this;
		}

		Duration& Duration::operator*=(F32 scalar)
		{
			mul(scalar);
			return *this;
		}

		Duration& Duration::operator/=(F32 divisor)
		{
			div(divisor);
			return *this;
		}

		Time add(const Time& t, const Duration& d)
		{
			return Time(t.value + d.span);
		}

		Time sub(const Time& t, const Duration& d)
		{
			return Time(t.value - d.span);
		}

		Duration sub(const Time& t1, const Time& t2)
		{
			return Duration(t1.value - t2.value);
		}

		Time operator+(const Time& t, const Duration& d)
		{
			return add(t, d);
		}

		Time operator+(const Duration& d, const Time& t)
		{
			return add(t, d);
		}

		Time operator-(const Time& t, const Duration& d)
		{
			return sub(t, d);
		}

		Duration operator-(const Time& t1, const Time& t2)
		{
			return sub(t1, t2);
		}

		Duration add(const Duration& d1, const Duration& d2)
		{
			return Duration(d1.span + d2.span);
		}

		Duration sub(const Duration& d1, const Duration& d2)
		{
			return Duration(d1.span - d2.span);
		}

		Duration mul(const Duration& d, F32 scalar)
		{
			return Duration(d.span * scalar);
		}

		Duration div(const Duration& d, F32 divisor)
		{
			return Duration(d.span / divisor);
		}

		Duration operator+(const Duration& d1, const Duration& d2)
		{
			return add(d1, d2);
		}

		Duration operator-(const Duration& d1, const Duration& d2)
		{
			return sub(d1, d2);
		}

		Duration operator*(const Duration& d, F32 scalar)
		{
			return mul(d, scalar);
		}

		Duration operator*(F32 scalar, const Duration& d)
		{
			return mul(d, scalar);
		}

		Duration operator/(const Duration& d, F32 scalar)
		{
			return div(d, scalar);
		}

		Duration operator/(F32 scalar, const Duration& d)
		{
			return div(d, scalar);
		}

	}
}