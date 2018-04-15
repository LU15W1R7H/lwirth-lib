#pragma once

#include "Standard.hpp"

#include "Math.hpp"

#include <iostream>

namespace lw
{
	template <size_t N>
	class Vector
	{
	private:
		f32 m_components[N];

	public:
		Vector()
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] = 0;
			}
		}

		template<typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0> //trick: enable_if returns typename-int, if first condition evaluations true -> otherwise the function returns error-type, which lets error-type = 0 throw an error
		Vector(Args... args) : m_components{ args... }
		{
		}

		Vector(f32 data[N])
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] = data[i];
			}
		}

		Vector(const Vector<N>& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] = v.m_components[i];
			}
		}

		f32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const
		{
			f32 r = 0;
			for (size_t i = 0; i < N; i++)
			{
				r += m_components[i] * m_components[i];
			}
			return r;
		}

		Vector<N>& norm()
		{
			f32 f = 1.f / mag();
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& setMag(f32 newMag)
		{
			f32 f = newMag / mag();
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& limit(f32 maxMag)
		{
			if (magSqd() > (maxMag * maxMag))
				setMag(maxMag);
		}

		inline f32& operator[](size_t index)
		{
			return m_components[index];
		}

		inline const f32& operator[](size_t index) const
		{
			return m_components[index];
		}

		Vector<N>& operator*=(f32 scalar)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= scalar;
			}
			return *this;
		}

		Vector<N>& operator/=(f32 scalar)
		{
			f32 f = 1.f / scalar;
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& operator+=(const Vector<N>& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] += v.m_components[i];
			}
			return *this;
		}

		Vector<N>& operator-=(const Vector<N>& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] -= v.m_components[i];
			}
			return *this;
		}
	};

	template<size_t N>
	bool operator==(const Vector<N>& v1, const Vector<N>& v2)
	{
		for (size_t i = 0; i < N; i++)
		{
			if (v1.m_components[i] != v2.m_components[i])
			{
				return false;
			}
		}
		return true;
	}

	template<size_t N>
	inline bool operator!=(const Vector<N>& v1, const Vector<N>& v2)
	{
		return !(v1 == v2);
	}

	template<size_t N>
	inline Vector<N> operator-(const Vector<N>& v)
	{
		Vector<N> r;
		for (size_t i = 0; i < N; i++)
		{
			r.m_components[i] = -v.m_components[i];
		}
		return r;
	}

	template<size_t N>
	inline Vector<N> operator+(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r = v1;
		r += v2;
		return r;
	}

	template<size_t N>
	inline Vector<N> operator-(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r = v1;
		r -= v2;
		return r;
	}

	template<size_t N>
	inline Vector<N> operator*(const Vector<N>& v, f32 scalar)
	{
		Vector<N> r = v;
		r *= scalar;
		return r;
	}

	template<size_t N>
	inline Vector<N> operator*(f32 scalar, const Vector<N>& v)
	{
		Vector<N> r = v;
		r *= scalar;
		return r;
	}

	template<size_t N>
	inline Vector<N> operator/(const Vector<N>& v, f32 scalar)
	{
		Vector<N> r = v;
		r /= scalar;
		return r;
	}

	template<size_t N>
	inline Vector<N> min(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r;
		for (size_t i = 0; i < N; i++)
		{
			r.m_components[i] = lw::min(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<size_t N>
	inline Vector<N> max(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r;
		for (size_t i = 0; i < N; i++)
		{
			r.m_components[i] = lw::max(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<size_t N>
	inline Vector<N> norm(const Vector<N>& v)
	{
		Vector<N> r = v;
		r.norm();
		return r;
	}

	template<size_t N>
	inline Vector<N> setMag(const Vector<N>& v, f32 mag)
	{
		Vector<N> r = v;
		r.setMag(mag);
		return r;
	}

	template<size_t N>
	inline f32 dot(const Vector<N>& v1, const Vector<N>& v2)
	{
		f32 r = 0;
		for (size_t i = 0; i < N; i++)
		{
			r += v1.m_components[i] * v2.m_components[i];
		}
		return r;
	}

	template<size_t N>
	inline f32 angle(const Vector<N>& v1, const Vector<N>& v2)
	{
		return std::acos((v1 * v2) / (v1.mag() * v2.mag()));
	}

	template<size_t N>
	inline std::ostream& operator<<(std::ostream& os, const Vector<N>& v)
	{
		os << "[";
		for (size_t i = 0; i < N; i++)
		{
			os << v.m_components[i];
			if (i != N - 1)
			{
				os << ", ";
			}
		}
		os << "]";
		return os;
	}

}

#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"