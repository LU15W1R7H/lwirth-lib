#pragma once

#include "Standard.hpp"

#include "Vector.hpp"
#include "Exceptions.hpp"

namespace lw
{
	template<class T>
	class vector_t<4, T>
	{
	public:
		f32 x;
		f32 y;
		f32 z;
		f32 w;

		vector_t()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
			this->w = 0;
		}

		vector_t(f32 x, f32 y, f32 z, f32 w)
		{
			this->x = x;
			this->y = y;
			this->z = z;
			this->w = w;
		}

		vector_t(f32 xyzw)
		{
			this->x = xyzw;
			this->y = xyzw;
			this->z = xyzw;
			this->w = xyzw;
		}

		vector_t(f32 data[4])
		{
			this->x = data[0];
			this->y = data[1];
			this->z = data[2];
			this->w = data[3];
		}

		vector_t(const vector_t<4, T>& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
			this->w = v.w;
		}

		friend vector_t<3, T> VectorByPoints(vector_t<3, T>&& start, vector_t<3, T>&& end)
		{
			return p2 - p1;
		}

		friend vector_t<3, T> VectorByPoints(const vector_t<3, T>& start, const vector_t<3, T>& end)
		{
			return p2 - p1;
		}

		f32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const
		{
			return x * x + y * y + z * z + w * w;
		}

		void norm()
		{
			if (magSqd() == 1.f)return;
			f32 f = 1.f / mag();
			x *= f;
			y *= f;
			z *= f;
			w *= f;
		}

		void setMag(f32 newMag)
		{
			if (magSqd() == newMag * newMag)return;
			f32 f = newMag / mag();
			x *= f;
			y *= f;
			z *= f;
			w *= f;
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
			case 3:
				return w;
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
			case 3:
				return w;
			default:
				throw IllegalIndexException();
			}
		}

		vector_t<4, T>& operator*=(f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			w *= scalar;
			return *this;
		}

		vector_t<4, T>& operator/=(f32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			w /= scalar;
			return *this;
		}

		vector_t<4, T>& operator+=(const vector_t<4, T>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		vector_t<4, T>& operator+=(vector_t<4, T>&& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			w += v.w;
			return *this;
		}

		vector_t<4, T>& operator-=(const vector_t<4, T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

		vector_t<4, T>& operator-=(vector_t<4, T>&& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			w -= v.w;
			return *this;
		}

	};

	template<class T>
	bool operator==(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
	}

	template<class T>
	bool operator==(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z && v1.w == v2.w;
	}

	template<class T>
	bool operator!=(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return !(v1 == v2);
	}

	template<class T>
	bool operator!=(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return !(v1 == v2);
	}

	template<class T>
	vector_t<4, T> operator-(vector_t<4, T>&& v)
	{
		return (v *= -1.f);
	}

	template<class T>
	vector_t<4, T> operator-(const vector_t<4, T>& v)
	{
		auto r = v;
		return (r *= -1.f);
	}

	template<class T>
	vector_t<4, T> operator+(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return(v1 += v2);
	}

	template<class T>
	vector_t<4, T> operator+(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		auto r = v1;
		return (r += v2);
	}

	template<class T>
	vector_t<4, T> operator-(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return (v1 += v2);
	}

	template<class T>
	vector_t<4, T> operator-(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		auto r = v1;
		return (r -= v2);
	}

	template<class T>
	vector_t<4, T> operator*(vector_t<4, T>&& v, f32 scalar)
	{
		return (v *= scalar);
	}

	template<class T>
	vector_t<4, T> operator*(const vector_t<4, T>& v, f32 scalar)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<class T>
	vector_t<4, T> operator*(f32 scalar, vector_t<4, T>&& v)
	{
		return (v *= scalar);
	}

	template<class T>
	vector_t<4, T> operator*(f32 scalar, const vector_t<4, T>& v)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<class T>
	vector_t<4, T> operator/(vector_t<4, T>&& v, f32 scalar)
	{
		return (v /= scalar);
	}

	template<class T>
	vector_t<4, T> operator/(const vector_t<4, T>& v, f32 scalar)
	{
		auto r = v;
		return (r /= scalar);
	}


	template<class T>
	vector_t<4, T> min(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return vector_t<4, T>(lw::min(v1.x, v2.x), lw::min(v1.y, v2.y), lw::min(v1.z, v2.z), lw::min(v1.w, v2.w));
	}

	template<class T>
	vector_t<4, T> max(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return vector_t<4, T>(lw::max(v1.x, v2.x), lw::max(v1.y, v2.y), lw::max(v1.z, v2.z), lw::max(v1.w, v2.w));
	}

	template<class T>
	vector_t<4, T> norm(vector_t<4, T>&& v)
	{
		v.norm();
		return v;
	}

	template<class T>
	vector_t<4, T> norm(const vector_t<4, T>& v)
	{
		auto r = v;
		r.norm();
		return r;
	}

	template<class T>
	vector_t<4, T> setMag(vector_t<4, T>&& v, f32 mag)
	{
		v.setMag(mag);
		return v;
	}

	template<class T>
	vector_t<4, T> setMag(const vector_t<4, T>& v, f32 mag)
	{
		auto r = v;
		r.setMag(mag);
		return r;
	}

	template<class T>
	f32 dot(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	template<class T>
	f32 dot(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
	}

	template<class T>
	f32 angle(vector_t<4, T>&& v1, vector_t<4, T>&& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<class T>
	f32 angle(const vector_t<4, T>& v1, const vector_t<4, T>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, const vector_t<4, T>& v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << ", " << v.w << "]";
		return os;
	}


}



