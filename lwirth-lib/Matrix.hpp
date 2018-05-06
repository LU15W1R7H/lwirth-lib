#pragma once

#include "Standard.hpp"

#include <type_traits>
#include "Vector.hpp"

namespace lw
{
	template<size_t R, size_t C, class T>
	class matrix_t
	{
		static_assert(R > 0 && C > 0, "RowCount and ColumnCount must be greater than 0.");

	private:
		T m_elements[R][C]; //first index = row, second index = column
	
	public:

		//creates identity matrix
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

		static matrix_t<R, C, T>&& identity()
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
		}

		static size_t getRowAmount()
		{
			return R;
		}

		static size_t getColumnAmount()
		{
			return C;
		}

		static bool isSquare()
		{
			return R == C;
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

	template<size_t R1, size_t C1, class T1, size_t R2, size_t C2, class T2>
	bool operator==(matrix_t<R1, C1, T1>&& m1, matrix_t<R2, C2, T2>&& m2)
	{
		if (T1 != T2 || R1 != R2 || C1 != C2)
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

	template<size_t R1, size_t C1, class T1, size_t R2, size_t C2, class T2>
	bool operator!=(matrix_t<R1, C1, T1>&& m1, matrix_t<R2, C2, T2>&& m2)
	{
		return !(LW_FORWARD(m1) == LW_FORWARD(m2));
	}

	template<size_t R, size_t C, class T>
	decltype(auto) operator*(matrix_t<R, C, T>&& m, f32 scalar)
	{
		auto r = LW_FORWARD(m);
		r *= scalar;
		return LW_FORWARD(r);
	}

	template<size_t R, size_t C, class T>
	decltype(auto) operator*(f32 scalar, matrix_t<R, C, T>&& m)
	{
		auto r = LW_FORWARD(m);
		r *= scalar;
		return LW_FORWARD(r);
	}

	template<size_t R, size_t C, class T>
	decltype(auto) operator+(matrix_t<R, C, T>&& m1, matrix_t<R, C, T>&& m2)
	{
		auto r = LW_FORWARD(m1);
		r += LW_FORWARD(m2);
		return LW_FORWARD(r);
	}

	template<size_t R, size_t C, class T>
	decltype(auto) operator-(matrix_t<R, C, T>&& m1, matrix_t<R, C, T>&& m2)
	{
		auto r = LW_FORWARD(m1);
		r -= LW_FORWARD(m2);
		return LW_FORWARD(r);
	}

	template<size_t R1, class T1, size_t C1R2, size_t C2, class T2>
	decltype(auto) operator*(matrix_t<R1, C1R2, T1>&& m1, matrix_t<C1R2, C2, T2>&& m2)
	{
		matrix_t<R1, C2, decltype(T1*T2)> r;
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
		return std::move(r);
	}

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
