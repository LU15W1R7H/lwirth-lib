#pragma once
#pragma once

#include "Standard.hpp"

#include "Matrix.hpp"


namespace lw
{
	template<class T>
	class matrix_t<2, 2, T>
	{
	private:
		f32 m_elements[2][2]; //first index = row, second index = column

	public:
		static const matrix_t<2, 2> zero;
		static const matrix_t<2, 2> identity;

		//creates identity matrix
		matrix_t()
		{
			for (auto& row : m_elements)
			{
				for (float& element : row)
				{
					element = 0;
				}
			}
		}

		matrix_t(f32 values[2][2])
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					m_elements[i][j] = values[i][j];
				}
			}
		}

		matrix_t(f32 r0c0, f32 r0c1, f32 r1c0, f32 r1c1)
		{
			m_elements[0][0] = r0c0;
			m_elements[0][1] = r0c1;
			m_elements[1][0] = r1c0;
			m_elements[1][1] = r1c1;
		}

		static matrix_t<2, 2> createRotation(f32 radians)
		{
			matrix_t<2, 2> r;
			f32 sin = std::sin(radians);
			f32 cos = std::cos(radians);
			r[0][0] = cos;
			r[0][1] = -sin;
			r[1][0] = sin;
			r[1][1] = cos;

			return r;
		}

		matrix_t(const matrix_t<2, 2>& m)
		{
			for (size_t i = 0; i < 2; i++)
			{
				for (size_t j = 0; j < 2; j++)
				{
					m_elements[i][j] = m.m_elements[i][j];
				}
			}
		}

		f32* operator[](size_t rowIndex)
		{
			return m_elements[rowIndex];
		}

		const f32* operator[](size_t rowIndex) const
		{
			return m_elements[rowIndex];
		}

		matrix_t<2, 2>& operator*=(f32 scalar)
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

		matrix_t<2, 2>& operator+=(matrix_t<2, 2>&& other)
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

		matrix_t<2, 2>& operator-=(matrix_t<2, 2>&& other)
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
}



