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
			Time(F32 value);

			F32 value;

			void add(const Duration& d);
			void sub(const Duration& d);

			Time& operator+=(const Duration& d);
			Time& operator-=(const Duration& d);
		};

		class API Duration
		{
		public:
			Duration();
			Duration(F32 span);

			F32 span = 0;

			void add(const Duration& d);
			void sub(const Duration& d);
			void mul(F32 scalar);
			void div(F32 divisor);

			Duration& operator+=(const Duration& d);
			Duration& operator-=(const Duration& d);
			Duration& operator*=(F32 scalar);
			Duration& operator/=(F32 divisor);
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
		API Duration mul(const Duration& d, F32 scalar);
		API Duration div(const Duration& d, F32 divisor);
		 
		API Duration operator+(const Duration& d1, const Duration& d2);
		API Duration operator-(const Duration& d1, const Duration& d2);
		API Duration operator*(const Duration& d, F32 scalar);
		API Duration operator*(F32 scalar, const Duration& d);
		API Duration operator/(const Duration& d, F32 scalar);
		API Duration operator/(F32 scalar, const Duration& d);
		
	}
}