#include "stdafx.hpp"

#include "Angle.hpp"

#include <cmath>

namespace lw
{
	const F32 Angle::degToRad = PI / 180.f;
	const F32 Angle::radToDeg = 1.f / degToRad;

	Angle::Angle()
		: m_radian(0.f)
	{
	}

	Angle::Angle(F32 _value, AngleMode _mode)
	{
		switch (_mode)
		{
		case RADIAN:
			m_radian = _value;
			break;
		case DEGREE:
			m_radian = degToRad * _value;
		}
	}

	void Angle::set(F32 _value, AngleMode _mode)
	{
		switch (_mode)
		{
		case RADIAN:
			m_radian = _value;
			break;
		case DEGREE:
			m_radian = degToRad * _value;
		}
	}

	F32 Angle::get(AngleMode _mode) const
	{
		switch (_mode)
		{
		case RADIAN:
			return m_radian;
			break;
		case DEGREE:
			return radToDeg * m_radian;
		}
	}

	void Angle::norm()
	{
		m_radian = std::fmod(m_radian, PI2);
	}

	Angle& Angle::mul(F32 scalar)
	{
		m_radian *= scalar;
		return *this;
	}

	Angle& Angle::div(F32 scalar)
	{
		m_radian /= scalar;
		return *this;
	}

	Angle& Angle::add(const Angle& other)
	{
		this->m_radian += other.m_radian;
		return *this;
	}

	Angle& Angle::sub(const Angle& other)
	{
		this->m_radian -= other.m_radian;
		return *this;
	}

	Angle& Angle::operator*=(F32 scalar)
	{
		return mul(scalar);
	}

	Angle& Angle::operator/=(F32 scalar)
	{
		return div(scalar);
	}

	Angle& Angle::operator+=(const Angle& other)
	{
		return add(other);
	}

	Angle& Angle::operator-=(const Angle& other)
	{
		return sub(other);
	}

	

	



	API bool equal(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian == a2.m_radian;
	}

	API bool notEqual(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian != a2.m_radian;
	}

	API bool greater(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian > a2.m_radian;
	}

	API bool less(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian < a2.m_radian;
	}

	API bool greaterEqual(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian >= a2.m_radian;
	}

	API bool lessEqual(const Angle& a1, const Angle& a2)
	{
		return a1.m_radian <= a2.m_radian;
	}

	API Angle neg(const Angle& a)
	{
		return Angle(-a.m_radian);
	}

	API Angle min(const Angle& a1, const Angle& a2)
	{
		return a1 < a2 ? a1 : a2;
	}

	API Angle max(const Angle& a1, const Angle& a2)
	{
		return a1 > a2 ? a1 : a2;
	}

	API Angle add(const Angle& a1, const Angle& a2)
	{
		return Angle(a1.m_radian + a2.m_radian);
	}

	API Angle sub(const Angle& a1, const Angle& a2)
	{
		return Angle(a1.m_radian - a2.m_radian);
	}

	API Angle mul(const Angle& a, F32 scalar)
	{
		return Angle(a.m_radian * scalar);
	}

	API Angle div(const Angle & a, F32 scalar)
	{
		return Angle(a.m_radian / scalar);
	}

	API Angle norm(const Angle& a)
	{
		return Angle(std::fmod(a.m_radian, PI2));
	}


	API bool operator==(const Angle& a1, const Angle& a2)
	{
		return equal(a1, a2);
	}

	API bool operator!=(const Angle& a1, const Angle& a2)
	{
		return notEqual(a1, a2);
	}

	API bool operator>(const Angle& a1, const Angle& a2)
	{
		return greater(a1, a2);
	}

	API bool operator<(const Angle& a1, const Angle& a2)
	{
		return less(a1, a2);
	}

	API bool operator>=(const Angle& a1, const Angle& a2)
	{
		return greaterEqual(a1, a2);
	}

	API bool operator<=(const Angle& a1, const Angle& a2)
	{
		return lessEqual(a1, a2);
	}

	API Angle operator-(const Angle& a)
	{
		return neg(a);
	}

	API Angle operator+(const Angle& a1, const Angle& a2)
	{
		return add(a1, a2);
	}

	API Angle operator-(const Angle& a1, const Angle& a2)
	{
		return sub(a1, a2);
	}

	API Angle operator*(const Angle& a, F32 scalar)
	{
		return mul(a, scalar);
	}

	API Angle operator*(F32 scalar, const Angle& a)
	{
		return mul(a, scalar);
	}

	API Angle operator/(const Angle& a, F32 scalar)
	{
		return div(a, scalar);
	}

	API F32 sin(const Angle& a)
	{
		return std::sinf(a.m_radian);
	}

	API F32 cos(const Angle& a)
	{
		return std::cosf(a.m_radian);
	}

	API F32 tan(const Angle& a)
	{
		return std::tanf(a.m_radian);
	}

	API F32 sinh(const Angle& a)
	{
		return std::sinhf(a.m_radian);
	}

	API F32 cosh(const Angle& a)
	{
		return std::coshf(a.m_radian);
	}

	API F32 tanh(const Angle & a)
	{
		return std::tanhf(a.m_radian);
	}

	API Angle asin(F32 x)
	{
		return Angle(std::asinf(x));
	}

	API Angle acos(F32 x)
	{
		return Angle(std::acosf(x));
	}

	API Angle atan(F32 x)
	{
		return Angle(std::atanf(x));
	}

	API Angle atan2(F32 y, F32 x)
	{
		return Angle(std::atan2f(y, x));
	}

	API Angle asinh(F32 x)
	{
		return Angle(std::asinhf(x));
	}

	API Angle acosh(F32 x)
	{
		return Angle(std::acoshf(x));
	}

	API Angle atanh(F32 x)
	{
		return Angle(std::atanhf(x));
	}

	

}

