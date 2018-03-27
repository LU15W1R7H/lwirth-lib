#pragma once

#include "Standard.hpp"

#include "Angle.hpp"
#include "Math.hpp"

#include <iostream>

namespace lw
{
	template <Size N>
	class Vector
	{
	private:
		F32 m_components[N];

	public:
		Vector()
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] = 0;
			}
		}

		template<typename... Args, typename std::enable_if<sizeof...(Args) == N, int>::type = 0> //trick: enable_if returns typename-int, if first condition evaluations true -> otherwise the function returns error-type, which lets error-type = 0 throw an error
		Vector(Args... args) : m_components{ args... }
		{
		}

		Vector(F32 data[N])
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] = data[i];
			}
		}

		Vector(const Vector<N>& v)
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] = v.m_components[i];
			}
		}

		F32 mag() const
		{
			return lw::sqrt(magSqd());
		}

		F32 magSqd() const
		{
			F32 r = 0;
			for (Size i = 0; i < N; i++)
			{
				r += m_components[i] * m_components[i];
			}
			return r;
		}

		Vector<N>& norm()
		{
			F32 f = 1.f / mag();
			for (Size i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& setMag(F32 newMag)
		{
			F32 f = newMag / mag();
			for (Size i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& limit(F32 maxMag)
		{
			if (magSqd() > (maxMag * maxMag))
				setMag(maxMag);
		}

		inline F32& operator[](Size index)
		{
			return m_components[index];
		}

		inline const F32& operator[](Size index) const
		{
			return m_components[index];
		}

		Vector<N>& operator*=(F32 scalar)
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] *= scalar;
			}
			return *this;
		}

		Vector<N>& operator/=(F32 scalar)
		{
			F32 f = 1.f / scalar;
			for (Size i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		Vector<N>& operator+=(const Vector<N>& v)
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] += v.m_components[i];
			}
			return *this;
		}

		Vector<N>& operator-=(const Vector<N>& v)
		{
			for (Size i = 0; i < N; i++)
			{
				m_components[i] -= v.m_components[i];
			}
			return *this;
		}
	};

	template<Size N>
	bool operator==(const Vector<N>& v1, const Vector<N>& v2)
	{
		for (Size i = 0; i < N; i++)
		{
			if (v1.m_components[i] != v2.m_components[i])
			{
				return false;
			}
		}
		return true;
	}

	template<Size N>
	inline bool operator!=(const Vector<N>& v1, const Vector<N>& v2)
	{
		return !(v1 == v2);
	}

	template<Size N>
	inline Vector<N> operator-(const Vector<N>& v)
	{
		Vector<N> r;
		for (Size i = 0; i < N; i++)
		{
			r.m_components[i] = -v.m_components[i];
		}
		return r;
	}

	template<Size N>
	inline Vector<N> operator+(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r = v1;
		r += v2;
		return r;
	}

	template<Size N>
	inline Vector<N> operator-(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r = v1;
		r -= v2;
		return r;
	}

	template<Size N>
	inline Vector<N> operator*(const Vector<N>& v, F32 scalar)
	{
		Vector<N> r = v;
		r *= scalar;
		return r;
	}

	template<Size N>
	inline Vector<N> operator*(F32 scalar, const Vector<N>& v)
	{
		Vector<N> r = v;
		r *= scalar;
		return r;
	}

	template<Size N>
	inline Vector<N> operator/(const Vector<N>& v, F32 scalar)
	{
		Vector<N> r = v;
		r /= scalar;
		return r;
	}

	template<Size N>
	inline Vector<N> min(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r;
		for (Size i = 0; i < N; i++)
		{
			r.m_components[i] = lw::min(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<Size N>
	inline Vector<N> max(const Vector<N>& v1, const Vector<N>& v2)
	{
		Vector<N> r;
		for (Size i = 0; i < N; i++)
		{
			r.m_components[i] = lw::max(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<Size N>
	inline Vector<N> norm(const Vector<N>& v)
	{
		Vector<N> r = v;
		r.norm();
		return r;
	}

	template<Size N>
	inline Vector<N> setMag(const Vector<N>& v, F32 mag)
	{
		Vector<N> r = v;
		r.setMag(mag);
		return r;
	}

	template<Size N>
	inline F32 dot(const Vector<N>& v1, const Vector<N>& v2)
	{
		F32 r = 0;
		for (Size i = 0; i < N; i++)
		{
			r += v1.m_components[i] * v2.m_components[i];
		}
		return r;
	}

	template<Size N>
	inline Angle angle(const Vector<N>& v1, const Vector<N>& v2)
	{
		return lw::acos((v1 * v2) / (v1.mag() * v2.mag()));
	}

	template<Size N>
	inline std::ostream& operator<<(std::ostream& os, const Vector<N>& v)
	{
		os << "[";
		for (Size i = 0; i < N; i++)
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
