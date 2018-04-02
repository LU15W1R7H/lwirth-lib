#pragma once

#include "Standard.hpp"

#include <type_traits>
#include "Vector.hpp"

namespace lw
{
	template<Size R, Size C>
	class Matrix
	{
	private:
		F32 m_elements[R][C]; //first index = row, second index = column
	
	public:
		static const Matrix<R, C> zero;
		static const Matrix<R, C> identity;

		//creates identity matrix
		Matrix()
		{
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
				{
					m_elements[i][j] = 0;
				}
			}
		}

		Matrix(F32 values[R][C])
		{
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
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
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
				{
					m_elements[i][j] = m.m_elements[i][j];
				}
			}
		}

		inline F32* operator[](Size rowIndex)
		{
			return m_elements[rowIndex];
		}

		inline const F32* operator[](Size rowIndex) const
		{
			return m_elements[rowIndex];
		}

		Matrix<R, C>& operator*=(F32 scalar)
		{
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
				{
					m_elements[i][j] *= scalar;
				}
			}
			return *this;
		}

		Matrix<R, C>& operator+=(const Matrix<R, C>& other)
		{
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
				{
					m_elements[i][j] += other.m_elements[i][j];
				}
			}
			return *this;
		}

		Matrix<R, C>& operator-=(const Matrix<R, C>& other)
		{
			for (Size i = 0; i < R; i++)
			{
				for (Size j = 0; j < C; j++)
				{
					m_elements[i][j] -= other.m_elements[i][j];
				}
			}
			return *this;
		}

	};

	template<Size R1, Size C1, Size R2, Size C2>
	inline bool operator==(const Matrix<R1, C1>& m1, const Matrix<R2, C2>& m2)
	{
		if (R1 != R2 || C1 != C2)
		{
			return false;
		}
		for (Size i = 0; i < R1; i++)
		{
			for (Size j = 0; j < C1; j++)
			{
				if (m1[i][j] != m2[i][j])return false;
			}
		}
		return true;
	}

	template<Size R1, Size C1, Size R2, Size C2>
	inline bool operator!=(const Matrix<R1, C1>& m1, const Matrix<R2, C2>& m2)
	{
		return !(m1 == m2);
	}

	template<Size R, Size C>
	inline Matrix<R, C> operator*(const Matrix<R, C>& m, F32 scalar)
	{
		Matrix<R, C> r = m1;
		r *= scalar;
		return r;
	}

	template<Size R, Size C>
	inline Matrix<R, C> operator*(F32 scalar, const Matrix<R, C>& m)
	{
		Matrix<R, C> r = m1;
		r *= scalar;
		return r;
	}

	template<Size R, Size C>
	inline Matrix<R, C> operator+(const Matrix<R, C>& m1, const Matrix<R, C>& m2)
	{
		Matrix<R, C> r = m1;
		r += m2;
		return r;
	}

	template<Size R, Size C>
	inline Matrix<R, C> operator-(const Matrix<R, C>& m1, const Matrix<R, C>& m2)
	{
		Matrix<R, C> r = m1;
		r -= m2;
		return r;
	}

	template<Size R1, Size C1R2, Size C2>
	inline Matrix<R1, C2> operator*(const Matrix<R1, C1R2>& m1, const Matrix<C1R2, C2>& m2)
	{
		Matrix<R1, C2> r;
		for (Size i = 0; i < R1; i++)
		{
			for (Size j = 0; j < C2; j++)
			{
				F32 v = 0;
				for (Size k = 0; k < C1R2; k++)
				{
					v += m1[i][k] * m2[k][j];
				}
				r[i][j] = v;
			}
		}
		return r;
	}

	template<Size R, Size C>
	inline Matrix<R, C> pow(const Matrix<R, C>& m, Size exponent)
	{
		auto r = Matrix<R, C>::identity;
		for (Size i = 0; i < exponent; i++)
		{
			r = m * m;
		}
		return r;
	}

	template<Size R, Size C>
	inline Vector<C> operator*(const Matrix<R, C>& m, const Vector<R>& v)
	{
		Vector<C> r;
		for (Size i = 0; i < R; i++)
		{
			for (Size j = 0; j < C; j++)
			{
				r[i] += m[i][j] * v[j];
			}
		}
		return r;
	}

	template<Size R, Size C>
	inline Matrix<C, R> transpose(const Matrix<R, C>& m)
	{
		Matrix<C, R> r;
		for (Size i = 0; i < R; i++)
		{
			for (Size j = 0; j < C; j++)
			{
				r[j][i] = m[i][j];
			}
		}
		return r;
	}
}


#include "Matrix2x2.hpp"
#include "Matrix4x4.hpp"
