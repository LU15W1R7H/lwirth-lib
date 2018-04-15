#pragma once

#include "Standard.hpp"

#include <type_traits>
#include "Vector.hpp"

namespace lw
{
	template<size_t R, size_t C>
	class Matrix
	{
	private:
		f32 m_elements[R][C]; //first index = row, second index = column
	
	public:
		static const Matrix<R, C> zero;
		static const Matrix<R, C> identity;

		//creates identity matrix
		Matrix()
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] = 0;
				}
			}
		}

		Matrix(f32 values[R][C])
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] = values[i][j];
				}
			}
		}

		template <typename ...Args, typename std::enable_if<sizeof...(Args) == R * C, int>::type = 0>
		constexpr Matrix(const Args&... args) : m_elements{ args... }
		{
		}

		Matrix(const Matrix<R, C>& m)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] = m.m_elements[i][j];
				}
			}
		}

		inline f32* operator[](size_t rowIndex)
		{
			return m_elements[rowIndex];
		}

		inline const f32* operator[](size_t rowIndex) const
		{
			return m_elements[rowIndex];
		}

		Matrix<R, C>& operator*=(f32 scalar)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] *= scalar;
				}
			}
			return *this;
		}

		Matrix<R, C>& operator+=(const Matrix<R, C>& other)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] += other.m_elements[i][j];
				}
			}
			return *this;
		}

		Matrix<R, C>& operator-=(const Matrix<R, C>& other)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] -= other.m_elements[i][j];
				}
			}
			return *this;
		}

	};

	template<size_t R1, size_t C1, size_t R2, size_t C2>
	inline bool operator==(const Matrix<R1, C1>& m1, const Matrix<R2, C2>& m2)
	{
		if (R1 != R2 || C1 != C2)
		{
			return false;
		}
		for (size_t i = 0; i < R1; i++)
		{
			for (size_t j = 0; j < C1; j++)
			{
				if (m1[i][j] != m2[i][j])return false;
			}
		}
		return true;
	}

	template<size_t R1, size_t C1, size_t R2, size_t C2>
	inline bool operator!=(const Matrix<R1, C1>& m1, const Matrix<R2, C2>& m2)
	{
		return !(m1 == m2);
	}

	template<size_t R, size_t C>
	inline Matrix<R, C> operator*(const Matrix<R, C>& m, f32 scalar)
	{
		Matrix<R, C> r = m1;
		r *= scalar;
		return r;
	}

	template<size_t R, size_t C>
	inline Matrix<R, C> operator*(f32 scalar, const Matrix<R, C>& m)
	{
		Matrix<R, C> r = m1;
		r *= scalar;
		return r;
	}

	template<size_t R, size_t C>
	inline Matrix<R, C> operator+(const Matrix<R, C>& m1, const Matrix<R, C>& m2)
	{
		Matrix<R, C> r = m1;
		r += m2;
		return r;
	}

	template<size_t R, size_t C>
	inline Matrix<R, C> operator-(const Matrix<R, C>& m1, const Matrix<R, C>& m2)
	{
		Matrix<R, C> r = m1;
		r -= m2;
		return r;
	}

	template<size_t R1, size_t C1R2, size_t C2>
	inline Matrix<R1, C2> operator*(const Matrix<R1, C1R2>& m1, const Matrix<C1R2, C2>& m2)
	{
		Matrix<R1, C2> r;
		for (size_t i = 0; i < R1; i++)
		{
			for (size_t j = 0; j < C2; j++)
			{
				f32 v = 0;
				for (size_t k = 0; k < C1R2; k++)
				{
					v += m1[i][k] * m2[k][j];
				}
				r[i][j] = v;
			}
		}
		return r;
	}

	template<size_t R, size_t C>
	inline Matrix<R, C> pow(const Matrix<R, C>& m, size_t exponent)
	{
		auto r = Matrix<R, C>::identity;
		for (size_t i = 0; i < exponent; i++)
		{
			r = m * m;
		}
		return r;
	}

	template<size_t R, size_t C>
	inline Vector<C> operator*(const Matrix<R, C>& m, const Vector<R>& v)
	{
		Vector<C> r;
		for (size_t i = 0; i < R; i++)
		{
			for (size_t j = 0; j < C; j++)
			{
				r[i] += m[i][j] * v[j];
			}
		}
		return r;
	}

	template<size_t R, size_t C>
	inline Matrix<C, R> transpose(const Matrix<R, C>& m)
	{
		Matrix<C, R> r;
		for (size_t i = 0; i < R; i++)
		{
			for (size_t j = 0; j < C; j++)
			{
				r[j][i] = m[i][j];
			}
		}
		return r;
	}
}


#include "Matrix2x2.hpp"
#include "Matrix4x4.hpp"
