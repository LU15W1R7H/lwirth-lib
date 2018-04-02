#pragma once

#include "Standard.hpp"

#include "Vector.hpp"
#include "Exceptions.hpp"

namespace lw
{
	template<>
	class Vector<4>
	{
	public:
		F32 x;
		F32 y;
		F32 z;
		F32 w;

		Vector()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		Vector(F32 x, F32 y, F32 z, F32 w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		Vector(F32 xyzw)
		{
			this->x = xyzw;
			this->y = xyzw;
			this->z = xyzw;
			this->w = xyzw;
		}

		Vector(F32 data[4])
		{
			this->x = data[0];
			this->y = data[1];
			this->z = data[2];
			this->w = data[3];
		}

		Vector(const Vector<4>& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
		}

		F32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		F32 magSqd() const
		{
			return x * x + y * y + z * z + w * w;
		}

		Vector<4>& norm()
		{
			F32 f = 1.f / mag();
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		Vector<4>& setMag(F32 newMag)
		{
			F32 f = newMag / mag();
			x *= f;
			y *= f;
			z *= f;
			w *= f;
			return *this;
		}

		F32& operator[](Size index)
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw IllegalIndexException();
			}
		}

		const F32& operator[](Size index) const
		{
			switch (index)
			{
			case 0:
				return x;
			case 1:
				return y;
			case 2:
				return z;
			case 3:
				return w;
			default:
				throw IllegalIndexException();
			}
		}

		Vector<4>& operator*=(F32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		Vector<4>& operator/=(F32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		Vector<4>& operator+=(const Vector<4>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		Vector<4>& operator-=(const Vector<4>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

	};

	typedef Vector<4> Vector4;

	inline bool operator==(const Vector<4>& v1, const Vector<4>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
	}

	inline bool operator!=(const Vector<4>& v1, const Vector<4>& v2)
	{
		return !(v1 == v2);
	}

	inline Vector<4> operator-(const Vector<4>& v)
	{
		return Vector<4>(-v.x, -v.y, -v.z, -v.w);
	}

	inline Vector<4> operator+(const Vector<4>& v1, const Vector<4>& v2)
	{
		Vector<4> r = v1;
		r += v2;
		return r;
	}

	inline Vector<4> operator-(const Vector<4>& v1, const Vector<4>& v2)
	{
		Vector<4> r = v1;
		r -= v2;
		return r;
	}

	inline Vector<4> operator*(const Vector<4>& v, F32 scalar)
	{
		Vector<4> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<4> operator*(F32 scalar, const Vector<4>& v)
	{
		Vector<4> r = v;
		r *= scalar;
		return r;
	}

	inline Vector<4> operator/(const Vector<4>& v, F32 scalar)
	{
		Vector<4> r = v;
		r /= scalar;
		return r;
	}

	inline Vector<4> min(const Vector<4>& v1, const Vector<4>& v2)
	{
		return Vector<4>(lw::min(v1.x, v2.x), lw::min(v1.y, v2.y), lw::min(v1.z, v2.z), lw::min(v1.w, v2.w));
	}

	inline Vector<4> max(const Vector<4>& v1, const Vector<4>& v2)
	{
		return Vector<4>(lw::max(v1.x, v2.x), lw::max(v1.y, v2.y), lw::max(v1.z, v2.z), lw::max(v1.w, v2.w));
	}

	inline Vector<4> norm(const Vector<4>& v)
	{
		Vector<4> r = v;
		r.norm();
		return r;
	}

	inline Vector<4> setMag(const Vector<4>& v, F32 mag)
	{
		Vector<4> r = v;
		r.setMag(mag);
		return r;
	}

	inline F32 dot(const Vector<4>& v1, const Vector<4>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	inline F32 angle(const Vector<4>& v1, const Vector<4>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	inline std::ostream& operator<<(std::ostream& os, const Vector<4>& v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
		return os;
	}

	using Vec4 = Vector<4>;

}



