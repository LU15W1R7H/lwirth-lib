#pragma once

#include "Standard.hpp"

#include <type_traits>
#include "Vector.hpp"

namespace lw
{
	//matrix base class
	

	//universal matrix
	template<size_t R, size_t C, class T>
	class matrix_t
	{
		static_assert(R > 0 && C > 0, "RowCount and ColumnCount must be greater than 0.");

	private:
		T m_elements[R][C]; //first index = row, second index = column
	
	public:

		//creates zero matrix
		matrix_t()
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m_elements[i][j] = 0;
				}
			}
		}

		matrix_t(f32 values[R][C])
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
		matrix_t(const Args&... args) : m_elements{ args... }
		{
		}

		matrix_t(const matrix_t<R, C, T>& m)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
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

		static matrix_t<R, C, T> identity()
		{
			matrix_t<R, C, T> r;
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					if constexpr(i == j)
						r.m_elements[i][j] = 1;
					else
						r.m_elements[i][j] = 0;
				}
			}
			return r;
		}

		static size_t getRowCount()
		{
			return R;
		}

		static size_t getColumnCount()
		{
			return C;
		}

		static bool isSquare()
		{
			return false;
		}

		bool isSymmetric()
		{
			if (!isSquare())return false;

			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					if (i == j)continue;
					if (m_elements[i][j] != m_elements[j][i])return false;
				}
			}
			return true;
		}

		f32 det() const
		{
			
		}

		matrix_t<R, C, T>& operator*=(f32 scalar)
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

		matrix_t<R, C, T>& operator+=(const matrix_t<R, C, T>& other)
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

		matrix_t<R, C, T>& operator-=(const matrix_t<R, C, T>& other)
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

	

	template<size_t R, size_t C, class T>
	decltype(auto) pow(matrix_t<R, C, T>&& m, size_t exponent)
	{
		auto r = matrix_t<R, C, T>::identity;
		for (size_t i = 0; i < exponent; i++)
		{
			r = m * m;
		}
		return std::move(r);
	}

	template<size_t R, size_t C, class MT, class VT>
	decltype(auto) operator*(matrix_t<R, C, MT>&& m, vector_t<R, VT>&& v)
	{
		vector_t<C, decltype(MT*VT)> r;
		for (size_t i = 0; i < R; i++)
		{
			for (size_t j = 0; j < C; j++)
			{
				r[i] += m[i][j] * v[j];
			}
		}
		return std::move(r);
	}

	template<size_t R, size_t C, class T>
	decltype(auto) transpose(matrix_t<R, C, T>&& m)
	{
		matrix_t<C, R, T> r;
		for (size_t i = 0; i < R; i++)
		{
			for (size_t j = 0; j < C; j++)
			{
				r[j][i] = m[i][j];
			}
		}
		return std::move(r);
	}
};
