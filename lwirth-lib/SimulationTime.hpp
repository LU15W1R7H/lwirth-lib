#pragma once

#include "Standard.hpp"

namespace lw
{
	namespace phy
	{
		class Time;
		class Duration;
		class Clock;

		class API Time
		{
		public:
			Time();
			Time(f32 value);

			f32 value;

			void add(const Duration& d);
			void sub(const Duration& d);

			Time& operator+=(const Duration& d);
			Time& operator-=(const Duration& d);
		};

		class API Duration
		{
		public:
			Duration();
			Duration(f32 span);

			f32 span = 0;

			void add(const Duration& d);
			void sub(const Duration& d);
			void mul(f32 scalar);
			void div(f32 divisor);

			Duration& operator+=(const Duration& d);
			Duration& operator-=(const Duration& d);
			Duration& operator*=(f32 scalar);
			Duration& operator/=(f32 divisor);
		};

		class API Clock
		{
		public:
			Clock();
			Clock(Time time, Duration tickDuration = Duration());

			Time time;
			Duration tickDuration;

			Duration tick();
			Duration tick(Duration tickDuration);
		};

		
		
		API Time add(const Time& t, const Duration& d);
		API Time sub(const Time& t, const Duration& d);
		API Duration sub(const Time& t1, const Time& t2);

		API Time operator+(const Time& t, const Duration& d);
		API Time operator+(const Duration& d, const Time& t);
		API Time operator-(const Time& t, const Duration& d);
		API Duration operator-(const Time& t1, const Time& t2);

		API Duration add(const Duration& d1, const Duration& d2);
		API Duration sub(const Duration& d1, const Duration& d2);
		API Duration mul(const Duration& d, f32 scalar);
		API Duration div(const Duration& d, f32 divisor);
		 
		API Duration operator+(const Duration& d1, const Duration& d2);
		API Duration operator-(const Duration& d1, const Duration& d2);
		API Duration operator*(const Duration& d, f32 scalar);
		API Duration operator*(f32 scalar, const Duration& d);
		API Duration operator/(const Duration& d, f32 scalar);
		API Duration operator/(f32 scalar, const Duration& d);
		
	}
}