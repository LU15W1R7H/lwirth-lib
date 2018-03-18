#pragma once

#include "Standard.hpp"

#include "Math.hpp"

namespace lw
{
	enum AngleMode { RADIAN, DEGREE };

	class API Angle
	{
	public:
		Angle();
		Angle(F32 _value, AngleMode _mode = RADIAN);
		~Angle() = default;

		void set(F32 _value, AngleMode _mode = RADIAN);
		F32 get(AngleMode _mode = RADIAN) const;

		void norm();

		inline Angle& mul(F32 scalar);
		inline Angle& div(F32 scalar);
		inline Angle& add(const Angle& other);
		inline Angle& sub(const Angle& other);
		 
		inline Angle& operator*=(F32 scalar);
		inline Angle& operator/=(F32 scalar);
		inline Angle& operator+=(const Angle& other);
		inline Angle& operator-=(const Angle& other);

		F32 m_radian = 0.f;
	private:

		static const F32 degToRad;
		static const F32 radToDeg;
	};

	inline API bool equal(const Angle& a1, const Angle& a2);
	inline API bool notEqual(const Angle& a1, const Angle& a2);
	inline API bool greater(const Angle& a1, const Angle& a2);
	inline API bool less(const Angle& a1, const Angle& a2);
	inline API bool greaterEqual(const Angle& a1, const Angle& a2);
	inline API bool lessEqual(const Angle& a1, const Angle& a2);
	inline API Angle neg(const Angle& a);
	inline API Angle min(const Angle& a1, const Angle& a2);
	inline API Angle max(const Angle& a1, const Angle& a2);
	inline API Angle add(const Angle& a1, const Angle& a2);
	inline API Angle sub(const Angle& a1, const Angle& a2);
	inline API Angle mul(const Angle& a, F32 scalar);
	inline API Angle div(const Angle& a, F32 scalar);
	inline API Angle norm(const Angle& a);

	inline API bool operator==(const Angle& a1, const Angle& a2);
	inline API bool operator!=(const Angle& a1, const Angle& a2);
	inline API bool operator>(const Angle& a1, const Angle& a2);
	inline API bool operator<(const Angle& a1, const Angle& a2);
	inline API bool operator>=(const Angle& a1, const Angle& a2);
	inline API bool operator<=(const Angle& a1, const Angle& a2);
	inline API Angle operator-(const Angle& a);
	inline API Angle operator+(const Angle& a1, const Angle& a2);
	inline API Angle operator-(const Angle& a1, const Angle& a2);
	inline API Angle operator*(const Angle& a, F32 scalar);
	inline API Angle operator*(F32 scalar, const Angle& a);
	inline API Angle operator/(const Angle& a, F32 scalar);

	inline API F32 sin(const Angle& a);
	inline API F32 cos(const Angle& a);
	inline API F32 tan(const Angle& a);
	inline API F32 sinh(const Angle& a);
	inline API F32 cosh(const Angle& a);
	inline API F32 tanh(const Angle& a);
	inline API Angle asin(F32 x);
	inline API Angle acos(F32 x);
	inline API Angle atan(F32 x);
	inline API Angle atan2(F32 y, F32 x);
	inline API Angle asinh(F32 x);
	inline API Angle acosh(F32 x);
	inline API Angle atanh(F32 x);
}



