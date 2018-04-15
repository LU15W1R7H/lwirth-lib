#pragma once

#include "Standard.hpp"

#include "Vector.hpp"
#include "Exceptions.hpp"

namespace lw
{
	template<>
	class Vector<3>
	{
	public:
		f32 x;
		f32 y;
		f32 z;

		Vector()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		Vector(f32 x, f32 y, f32 z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		Vector(f32 xyz)
		{
			this->x = xyz;
			this->y = xyz;
			this->z = xyz;
		}

		Vector(f32 data[3])
		{
			this->x = data[0];
			this->y = data[1];
			this->z = data[2];
		}

		Vector(const Vector<3>& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
		}

		f32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const
		{
			return x * x + y * y + z * z;
		}

		Vector<3>& norm()
		{
			f32 f = 1.f / mag();
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		Vector<3>& setMag(f32 newMag)
		{
			f32 f = newMag / mag();
			x *= f;
			y *= f;
			z *= f;
			return *this;
		}

		f32& operator[](size_t index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				throw IllegalIndexException();
			}
		}

		const f32& operator[](size_t index) const
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			default:
				throw IllegalIndexException();
			}
		}

		Vector<3>& operator*=(f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		Vector<3>& operator/=(f32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		Vector<3>& operator+=(const Vector<3>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vector<3>& operator-=(const Vector<3>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	};


	inline bool operator==(const Vector<3>& v1, const Vector<3>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}

	inline bool operator!=(const Vector<3>& v1, const Vector<3>& v2)
	{
		return !(v1 == v2);
	}

	inline Vector<3> operator-(const Vector<3>& v)
	{
		return Vector<3>(-v.x, -v.y, -v.z);
	}

	inline Vector<3> operator+(const Vector<3>& v1, const Vector<3>& v2)
	{
		Vector<3> r = v1;
		r += v2;
		return r;
	}

	inline Vector<3> operator-(const Vector<3>& v1, const Vector<3>& v2)
	{
		Vector<3> r = v1;
		r -= v2;
		return r;
	}

	inline Vector<3> operator*(const Vector<3>& v, f32 scalar)
	{
		Vector<3> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<3> operator*(f32 scalar, const Vector<3>& v)
	{
		Vector<3> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<3> operator/(const Vector<3>& v, f32 scalar)
	{
		Vector<3> r = v;
		r /= scalar;
		return r;
	}

	inline Vector<3> min(const Vector<3>& v1, const Vector<3>& v2)
	{
		return Vector<3>(lw::min(v1.x, v2.x), lw::min(v1.y, v2.y), lw::min(v1.z, v2.z));
	}

	inline Vector<3> max(const Vector<3>& v1, const Vector<3>& v2)
	{
		return Vector<3>(lw::max(v1.x, v2.x), lw::max(v1.y, v2.y), lw::max(v1.z, v2.z));
	}

	inline Vector<3> norm(const Vector<3>& v)
	{
		Vector<3> r = v;
		r.norm();
		return r;
	}

	inline Vector<3> setMag(const Vector<3>& v, f32 mag)
	{
		Vector<3> r = v;
		r.setMag(mag);
		return r;
	}

	inline f32 dot(const Vector<3>& v1, const Vector<3>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	inline Vector<3> cross(const Vector<3>& v1, const Vector<3>& v2)
	{
		return Vector<3>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
			);
	}

	inline f32 angle(const Vector<3>& v1, const Vector<3>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector<3>& v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}

	using Vec3 = Vector<3>;

}



