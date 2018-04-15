#pragma once
#pragma once

#include "Standard.hpp"

#include "Matrix.hpp"
#include "Vector.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"

namespace lw
{
	template<>
	class Matrix<2, 2>
	{
	private:
		f32 m_elements[2][2]; //first index = row, second index = column

	public:
		static const Matrix<2, 2> zero;
		static const Matrix<2, 2> identity;

		//creates identity matrix
		Matrix()
		{
			for (auto& row : m_elements)
			{
				for (float& element : row)
				{
					element = 0;
				}
			}
		}

		Matrix(f32 values[2][2])
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					m_elements[i][j] = values[i][j];
				}
			}
		}

		Matrix(f32 r0c0, f32 r0c1, f32 r1c0, f32 r1c1)
		{
			m_elements[0][0] = r0c0;
			m_elements[0][1] = r0c1;
			m_elements[1][0] = r1c0;
			m_elements[1][1] = r1c1;
		}

		static Matrix<2, 2> createRotation(f32 radians)
		{
			Matrix<2, 2> r;
			f32 sin = std::sin(radians);
			f32 cos = std::cos(radians);
			r[0][0] = cos;
			r[0][1] = -sin;
			r[1][0] = sin;
			r[1][1] = cos;

			return r;
		}

		Matrix(const Matrix<2, 2>& m)
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
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

		Matrix<2, 2>& operator*=(f32 scalar)
		{
			for (auto& row : m_elements)
			{
				for (float& element : row)
				{
					element *= scalar;
				}
			}
			return *this;
		}

		Matrix<2, 2>& operator+=(const Matrix<2, 2>& other)
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					m_elements[i][j] += other.m_elements[i][j];
				}
			}
			return *this;
		}

		Matrix<2, 2>& operator-=(const Matrix<2, 2>& other)
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					m_elements[i][j] -= other.m_elements[i][j];
				}
			}
			return *this;
		}

	};

	inline bool operator==(const Matrix<2, 2>& m1, const Matrix<2, 2>& m2)
	{
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				if (m1[i][j] != m2[i][j])return false;
			}
		}
		return true;
	}

	inline bool operator!=(const Matrix<2, 2>& m1, const Matrix<2, 2>& m2)
	{
		return !(m1 == m2);
	}


	inline Matrix<2, 2> operator*(const Matrix<2, 2>& m, f32 scalar)
	{
		Matrix<2, 2> r = m;
		r *= scalar;
		return r;
	}


	inline Matrix<2, 2> operator*(f32 scalar, const Matrix<2, 2>& m)
	{
		Matrix<2, 2> r = m;
		r *= scalar;
		return r;
	}


	inline Matrix<2, 2> operator+(const Matrix<2, 2>& m1, const Matrix<2, 2>& m2)
	{
		Matrix<2, 2> r = m1;
		r += m2;
		return r;
	}


	inline Matrix<2, 2> operator-(const Matrix<2, 2>& m1, const Matrix<2, 2>& m2)
	{
		Matrix<2, 2> r = m1;
		r -= m2;
		return r;
	}

	inline Matrix<2, 2> operator*(const Matrix<2, 2>& m1, const Matrix<2, 2>& m2)
	{
		Matrix<2, 2> r;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				f32 v = 0;
				for (size_t k = 0; k < 2; k++)
				{
					v += m1[i][k] * m2[k][j];
				}
				r[i][j] = v;
			}
		}
		return r;
	}


	inline Matrix<2, 2> pow(const Matrix<2, 2>& m, size_t exponent)
	{
		auto r = Matrix<2, 2>::identity;
		for (size_t i = 0; i < exponent; i++)
		{
			r = m * m;
		}
		return r;
	}


	inline Vector<2> operator*(const Matrix<2, 2>& m, const Vector<2>& v)
	{
		Vector<2> r;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				r[i] += m[i][j] * v[j];
			}
		}
		return r;
	}


	inline Matrix<2, 2> transpose(const Matrix<2, 2>& m)
	{
		Matrix<2, 2> r;
		for (size_t i = 0; i < 2; i++)
		{
			for (size_t j = 0; j < 2; j++)
			{
				r[j][i] = m[i][j];
			}
		}
		return r;
	}

	using Mat2 = Matrix<2, 2>;
}



