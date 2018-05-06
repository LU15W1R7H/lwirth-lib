#pragma once

#include "Standard.hpp"

#include "qmath.hpp"

#include <iostream>

namespace lw
{
	template <size_t N, class T>
	class vector_t
	{
		static_assert(N > 0, "Vector of dimension 0 is illegal");
		static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

	private:
		T m_components[N];

	public:
		vector_t() noexcept
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] = 0;
			}
		}

		vector_t(const vector_t<N, T>& v) noexcept
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] = v.m_components[i];
			}
		}

		vector_t(vector_t<N, T>&& v) noexcept
		{
			m_components = std::move(v.m_components);
		}

		friend vector_t<N, T> VectorByPoints(vector_t<N, T>&& start, vector_t<N, T>&& end)
		{
			return end - start;
		}

		friend vector_t<N, T> VectorByPoints(const vector_t<N, T>& start, const vector_t<N, T>& end)
		{
			return end - start;
		}

		static size_t getDim() noexcept
		{
			return N;
		}

		f32 mag() const noexcept
		{
			return lw::sqrt(magSqd());
		}

		f32 magSqd() const noexcept
		{
			f32 r = 0;
			for (size_t i = 0; i < N; i++)
			{
				r += m_components[i] * m_components[i];
			}
			return r;
		}

		void norm() noexcept
		{
			if (magSqd() == 1.f)return;
			f32 f = 1.f / mag();
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		void setMag(f32 newMag) noexcept
		{
			if (magSqd() == newMag * newMag)return;
			f32 f = newMag / mag();
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		void limit(f32 maxMag) noexcept
		{
			if (magSqd() > (maxMag * maxMag))
				setMag(maxMag);
		}

		f32& operator[](size_t index)
		{
			return m_components[index];
		}

		const f32& operator[](size_t index) const
		{
			return m_components[index];
		}

		vector_t<N, T>& operator*=(f32 scalar) noexcept
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= scalar;
			}
			return *this;
		}

		vector_t<N, T>& operator/=(f32 scalar) noexcept
		{
			f32 f = 1.f / scalar;
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] *= f;
			}
			return *this;
		}

		vector_t<N, T>& operator+=(vector_t<N, T>&& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] += v.m_components[i];
			}
			return *this;
		}

		vector_t<N, T>& operator+=(const vector_t<N, T>& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] += v.m_components[i];
			}
			return *this;
		}

		vector_t<N, T>& operator-=(vector_t<N, T>&& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] -= v.m_components[i];
			}
			return *this;
		}

		vector_t<N, T>& operator-=(const vector_t<N, T>& v)
		{
			for (size_t i = 0; i < N; i++)
			{
				m_components[i] -= v.m_components[i];
			}
			return *this;
		}
	};

	template<size_t N, class T>
	bool operator==(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
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

	template<size_t N, class T>
	bool operator==(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
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

	template<size_t N, class T>
	bool operator!=(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
	{
		return !(v1 == v2);
	}

	template<size_t N, class T>
	bool operator!=(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		return !(v1 == v2);
	}

	template<size_t N, class T>
	vector_t<N, T> operator-(vector_t<N, T>&& v)
	{
		return (v *= -1.f);
	}

	template<size_t N, class T>
	vector_t<N, T> operator-(const vector_t<N, T>& v)
	{
		auto r = v;
		return (r *= -1.f);
	}

	template<size_t N, class T>
	vector_t<N, T> operator+(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
	{
		return(v1 += v2);
	}

	template<size_t N, class T>
	vector_t<N, T> operator+(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		auto r = v1;
		return (r += v2);
	}

	template<size_t N, class T>
	vector_t<N, T> operator-(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
	{
		return (v1 += v2);
	}

	template<size_t N, class T>
	vector_t<N, T> operator-(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		auto r = v1;
		return (r -= v2);
	}

	template<size_t N, class T>
	vector_t<N, T> operator*(vector_t<N, T>&& v, f32 scalar)
	{
		return (v *= scalar);
	}

	template<size_t N, class T>
	vector_t<N, T> operator*(const vector_t<N, T>& v, f32 scalar)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<size_t N, class T>
	vector_t<N, T> operator*(f32 scalar, vector_t<N, T>&& v)
	{
		return (v *= scalar);
	}

	template<size_t N, class T>
	vector_t<N, T> operator*(f32 scalar, const vector_t<N, T>& v)
	{
		auto r = v;
		return (r *= scalar);
	}

	template<size_t N, class T>
	vector_t<N, T> operator/(vector_t<N, T>&& v, f32 scalar)
	{
		return (v /= scalar);
	}

	template<size_t N, class T>
	vector_t<N, T> operator/(const vector_t<N, T>& v, f32 scalar)
	{
		auto r = v;
		return (r /= scalar);
	}


	template<size_t N, class T>
	vector_t<N, T> min(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		vector_t<N, T> r;
		for (size_t i = 0; i < N; i++)
		{
			r.m_components[i] = lw::min(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<size_t N, class T>
	vector_t<N, T> max(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		vector_t<N, T> r;
		for (size_t i = 0; i < N; i++)
		{
			r.m_components[i] = lw::max(v1.m_components[i], v2.m_components[i]);
		}
		return r;
	}

	template<size_t N, class T>
	bool collinear(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
	{
		f32 f = v1[0] / v2[0];
		for (size_t i = 1; i < N; i++)
		{
			if (v1[i] != f * v2[i])return false;
		}
		return true;
	}

	template<size_t N, class T>
	bool collinear(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		f32 f = v1[0] / v2[0];
		for (size_t i = 1; i < N; i++)
		{
			if (v1[i] != f * v2[i])return false;
		}
		return true;
	}

	template<size_t N, class T>
	vector_t<N, T> norm(vector_t<N, T>&& v)
	{
		v.norm();
		return v;
	}

	template<size_t N, class T>
	vector_t<N, T> norm(const vector_t<N, T>& v)
	{
		auto r = v;
		r.norm();
		return r;
	}

	template<size_t N, class T>
	vector_t<N, T> setMag(vector_t<N, T>&& v, f32 mag)
	{
		v.setMag(mag);
		return v;
	}

	template<size_t N, class T>
	vector_t<N, T> setMag(const vector_t<N, T>& v, f32 mag)
	{
		auto r = v;
		r.setMag(mag);
		return r;
	}

	template<size_t N, class T>
	f32 dot(vector_t<N, T>&& v1, vector_t<N, T>&& v2)
	{
		f32 r = 0;
		for (size_t i = 0; i < N; i++)
		{
			r += v1.m_components[i] * v2.m_components[i];
		}
		return r;
	}

	template<size_t N, class T>
	f32 dot(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		f32 r = 0;
		for (size_t i = 0; i < N; i++)
		{
			r += v1.m_components[i] * v2.m_components[i];
		}
		return r;
	}

	template<size_t N, class T>
	f32 angle(vector_t<N, T>&& v1,vector_t<N, T>&& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<size_t N, class T>
	f32 angle(const vector_t<N, T>& v1, const vector_t<N, T>& v2)
	{
		return std::acos(dot(v1, v2) / (v1.mag() * v2.mag()));
	}

	template<size_t N, class T>
	std::ostream& operator<<(const std::ostream& os, const vector_t<N, T>& v)
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