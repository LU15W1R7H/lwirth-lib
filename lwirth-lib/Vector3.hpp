#pragma once

#include "Standard.hpp"

#include "Vector.hpp"
#include "Exceptions.hpp"

namespace lw
{
	template<class T>
	class vector_t<3, T>
	{
	public:
		f32 x;
		f32 y;
		f32 z;

		vector_t()
		{
			this->x = 0;
			this->y = 0;
			this->z = 0;
		}

		vector_t(f32 x, f32 y, f32 z)
		{
			this->x = x;
			this->y = y;
			this->z = z;
		}

		vector_t(f32 xyz)
		{
			this->x = xyz;
			this->y = xyz;
			this->z = xyz;
		}

		vector_t(f32 data[3])
		{
			this->x = data[0];
			this->y = data[1];
			this->z = data[2];
		}

		vector_t(const vector_t<3, T>& v)
		{
			this->x = v.x;
			this->y = v.y;
			this->z = v.z;
		}

		friend vector_t<3, T> VectorByPoints(vector_t<3, T>&& start, vector_t<3, T>&& end)
		{
			return end - start;
		}

		friend vector_t<3, T> VectorByPoints(const vector_t<3, T>& start, const vector_t<3, T>& end)
		{
			return end - start;
		}

		f32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const
		{
			return x * x + y * y + z * z;
		}

		void norm()
		{
			if (magSqd() == 1.f)return;
			f32 f = 1.f / mag();
			x *= f;
			y *= f;
			z *= f;
		}

		void setMag(f32 newMag)
		{
			if (magSqd() == newMag * newMag)return;
			f32 f = newMag / mag();
			x *= f;
			y *= f;
			z *= f;
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

		vector_t<3, T>& operator*=(f32 scalar)
		{
			x *= scalar;
			y *= scalar;
			z *= scalar;
			return *this;
		}

		vector_t<3, T>& operator/=(f32 scalar)
		{
			x /= scalar;
			y /= scalar;
			z /= scalar;
			return *this;
		}

		vector_t<3, T>& operator+=(const vector_t<3, T>& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		vector_t<3, T>& operator+=(vector_t<3, T>&& v)
		{
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		vector_t<3, T>& operator-=(const vector_t<3, T>& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}

		vector_t<3, T>& operator-=(vector_t<3, T>&& v)
		{
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}
	};

	template<class T>
	bool operator==(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}

	template<class T>
	bool operator==(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return v1.x == v2.x && v1.y == v2.y && v1.z == v2.z;
	}

	template<class T>
	bool operator!=(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return !(v1 == v2);
	}

	template<class T>
	bool operator!=(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return !(v1 == v2);
	}

	template<class T>
	vector_t<3, T> operator-(vector_t<3, T>&& v)
	{
		return (v *= -1.f);
	}

	template<class T>
	vector_t<3, T> operator-(const vector_t<3, T>& v)
	{
		auto r = v;
		return (r *= -1.f);
	}

	template<class T>
	vector_t<3, T> operator+(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return(v1 += v2);
	}

	template<class T>
	vector_t<3, T> operator+(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		auto r = v1;
		return (r += v2);
	}

	template<class T>
	vector_t<3, T> operator-(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return (v1 += v2);
	}

	template<class T>
	vector_t<3, T> operator-(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		auto r = v1;
		return (r -= v2);
	}

	template<class T>
	vector_t<3, T> operator*(vector_t<3, T>&& v, f32 scalar)
	{
		return (v *= scalar);
	}

	template<class T>
	vector_t<3, T> operator*(const vector_t<3, T>& v, f32 scalar)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<class T>
	vector_t<3, T> operator*(f32 scalar, vector_t<3, T>&& v)
	{
		return (v *= scalar);
	}

	template<class T>
	vector_t<3, T> operator*(f32 scalar, const vector_t<3, T>& v)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<class T>
	vector_t<3, T> operator/(vector_t<3, T>&& v, f32 scalar)
	{
		return (v /= scalar);
	}

	template<class T>
	vector_t<3, T> operator/(const vector_t<3, T>& v, f32 scalar)
	{
		auto r = v;
		return (r /= scalar);
	}


	template<class T>
	vector_t<3, T> min(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return vector_t<3, T>(lw::min(v1.x, v2.x), lw::min(v1.y, v2.y), lw::min(v1.z, v2.z));
	}

	template<class T>
	vector_t<3, T> max(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return vector_t<3, T>(lw::max(v1.x, v2.x), lw::max(v1.y, v2.y), lw::max(v1.z, v2.z));

	}

	template<class T>
	vector_t<3, T> norm(vector_t<3, T>&& v)
	{
		v.norm();
		return v;
	}

	template<class T>
	vector_t<3, T> norm(const vector_t<3, T>& v)
	{
		auto r = v;
		r.norm();
		return r;
	}

	template<class T>
	vector_t<3, T> setMag(vector_t<3, T>&& v, f32 mag)
	{
		v.setMag(mag);
		return v;
	}

	template<class T>
	vector_t<3, T> setMag(const vector_t<3, T>& v, f32 mag)
	{
		auto r = v;
		r.setMag(mag);
		return r;
	}

	template<class T>
	f32 dot(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<class T>
	f32 dot(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}

	template<class T>
	vector_t<3, T> cross(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return vector_t<3, T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
			);
	}

	template<class T>
	vector_t<3, T> cross(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return vector_t<3, T>(
			v1.y * v2.z - v1.z * v2.y,
			v1.z * v2.x - v1.x * v2.z,
			v1.x * v2.y - v1.y * v2.x
			);
	}

	template<class T>
	f32 angle(vector_t<3, T>&& v1, vector_t<3, T>&& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<class T>
	f32 angle(const vector_t<3, T>& v1, const vector_t<3, T>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<class T>
	std::ostream& operator<<(std::ostream& os, const vector_t<3, T>& v)
	{
		os << "[" << v.x << ", " << v.y << ", " << v.z << "]";
		return os;
	}



}



