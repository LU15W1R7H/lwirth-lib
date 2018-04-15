#pragma once

#include "Standard.hpp"

#include "Vector.hpp"
#include "Exceptions.hpp"

namespace lw
{
	template<>
	class Vector<2>
	{
	public:
		f32 x;
		f32 y;

		Vector()
		{
			this->x = 0;
			this->y = 0;
		}

		Vector(f32 x, f32 y)
		{
			this->x = x;
			this->y = y;
		}

		Vector(f32 xy)
		{
			this->x = xy;
			this->y = xy;
		}

		Vector(f32 data[2])
		{
			this->x = data[0];
			this->y = data[1];
		}

		Vector(const Vector<2>& v)
		{
			this->x = v.x;
			this->y = v.y;
		}

		f32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const
		{
			return x * x + y * y;
		}

		Vector<2>& norm()
		{
			f32 f = 1.f / mag();
			x *= f;
			y *= f;
			return *this;
		}

		Vector<2>& setMag(f32 newMag)
		{
			f32 f = newMag / mag();
			x *= f;
			y *= f;
			return *this;
		}


		f32& operator[](u32 index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			default:
				throw IllegalIndexException();
			}
		}

		const f32& operator[](u32 index) const
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			default:
				throw IllegalIndexException();
			}
		}

		Vector<2>& operator*=(f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			return *this;
		}

		Vector<2>& operator/=(f32 scalar)
		{
			x /= scalar;
			y /= scalar;
			return *this;
		}

		Vector<2>& operator+=(const Vector<2>& v)
		{
			x += v.x;
			y += v.y;
			return *this;
		}

		Vector<2>& operator-=(const Vector<2>& v)
		{
			x -= v.x;
			y -= v.y;
			return *this;
		}

	};

	inline bool operator==(const Vector<2>& v1, const Vector<2>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y;
	}

	inline bool operator!=(const Vector<2>& v1, const Vector<2>& v2)
	{
		return !(v1 == v2);
	}

	inline Vector<2> operator-(const Vector<2>& v)
	{
		return Vector<2>(-v.x, -v.y);
	}

	inline Vector<2> operator+(const Vector<2>& v1, const Vector<2>& v2)
	{
		Vector<2> r = v1;
		r += v2;
		return r;
	}

	inline Vector<2> operator-(const Vector<2>& v1, const Vector<2>& v2)
	{
		Vector<2> r = v1;
		r -= v2;
		return r;
	}

	inline Vector<2> operator*(const Vector<2>& v, f32 scalar)
	{
		Vector<2> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<2> operator*(f32 scalar, const Vector<2>& v)
	{
		Vector<2> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<2> operator/(const Vector<2>& v, f32 scalar)
	{
		Vector<2> r = v;
		r /= scalar;
		return r;
	}

	inline Vector<2> min(const Vector<2>& v1, const Vector<2>& v2)
	{
		return Vector<2>(lw::min(v1.x, v2.x), lw::min(v1.y, v2.y));
	}

	inline Vector<2> max(const Vector<2>& v1, const Vector<2>& v2)
	{
		return Vector<2>(lw::max(v1.x, v2.x), lw::max(v1.y, v2.y));
	}

	inline Vector<2> norm(const Vector<2>& v)
	{
		Vector<2> r = v;
		r.norm();
		return r;
	}

	inline Vector<2> setMag(const Vector<2>& v, f32 mag)
	{
		Vector<2> r = v;
		r.setMag(mag);
		return r;
	}

	inline f32 dot(const Vector<2>& v1, const Vector<2>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y;
	}

	inline f32 angle(const Vector<2>& v1, const Vector<2>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector<2>& v)
	{
		os << "[" << v.x << ", " << v.y << "]";
		return os;
	}
	
	using Vec2 = Vector<2>;

}



