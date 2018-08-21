#pragma once

#include "../Standard.hpp"

#include <type_traits>
#include "../general/Exceptions.hpp"
#include "Vector.hpp"

namespace lw
{
	template<class T, size_t R, size_t C>
	class elements_t;

	template<class T, size_t R, size_t C>
	class matrix_base;

	template<class T, size_t R, size_t C>
	class matrix_t;

	template<class T, size_t S>
	class matrix_t<T, S, S>;

	template<class T, size_t R, size_t C>
    class elements_t
    {
    public:
        using Type = T;

        std::array<std::array<T, C>, R> elems;

        constexpr static size_t rows()
        {
            return R;
        }

        constexpr static size_t cols()
        {
            return C;
        }

        constexpr static size_t size()
        {
            return R * C;
        }
    };

	//never return matrix_base
    template<class T, size_t R, size_t C>
    class matrix_base : public elements_t<T, R, C>
    {
    public:
        using SELF = matrix_base<T, R, C>;
		using DERIVED = matrix_t<T, R, C>;
		using ELEMENTS = elements_t<T, R, C>;

        matrix_base()
            : ELEMENTS()
        {
        }

        matrix_base(const SELF& m)
            : ELEMENTS(m)
        {
        }

        SELF& operator=(const SELF& m)
        {
            ELEMENTS::operator=(m);
            return *this;
        }

        matrix_base(SELF&& m)
            : ELEMENTS(m)
        {
        }

		DERIVED& operator=(SELF&& m)
        {
            ELEMENTS::operator=(m);
            return *this;
        }

		matrix_base(const std::initializer_list<T>& il)
		{
			if(il.size() != R * C)throw IllSizedContainer("Initalizer list doesn't match matrix shape.");
			const T* ptr = il.begin();
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					this->elems[i][j] = *ptr;
					ptr++;
				}
			}
		}

        template<class ... Args, class SFINAE = std::enable_if_t<sizeof...(Args) == ELEMENTS::getSize()>>
        matrix_base(Args&& ... args)
            : ELEMENTS(std::forward<Args>(args)...)
        {
        }

        constexpr static bool isSquare()
        {
            return ELEMENTS::getRows() == ELEMENTS::getColumns();
        }

		friend DERIVED& operator+=(DERIVED& m1, const DERIVED& m2)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m1.elems[i][j] += m2.elems[i][j];
				}
			}
			return m1;
		}

		friend DERIVED& operator-=(DERIVED m1, const DERIVED& m2)
		{
			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					m1.elems[i][j] -= m2.elems[i][j];
				}
			}
			return m1;
		}

		std::string stringify() const noexcept
		{
			std::string s;

			for (size_t i = 0; i < R; i++)
			{
				for (size_t j = 0; j < C; j++)
				{
					s.append(std::to_string(this->elems[i][j]));
					if (j != C - 1)
					{
						s.append(", ");
					}
					else
					{
						s.append("\n");
					}
				}
			}
			return s;
		}
		

    };

	

    //standard matrix
    template<class T, size_t R, size_t C>
    class matrix_t : public matrix_base<T, R, C>
    {
		using matrix_base<T, R, C>::matrix_base;
    };

    //square matrix
    template<class T, size_t S>
    class matrix_t<T, S, S> : public matrix_base<T, S, S>
    {
		using matrix_base<T, S, S>::matrix_base;


		typename T det() const
		{
			//return determinant
		}
    };

	template<class T, size_t R, size_t C>
	matrix_t<T, R, C> operator+(const matrix_t<T, R, C>& m1, const matrix_t<T, R, C>& m2)
	{
		matrix_t<T, R, C> r = m1;
		return m1 += m2;
	}

	template<class T, size_t R, size_t C>
	matrix_t<T, R, C> operator-(const matrix_t<T, R, C>& m1, const matrix_t<T, R, C>& m2)
	{
		matrix_t<T, R, C> r = m1;
		return m1 -= m2;
	}

    template<typename T1, typename T2, size_t R1, size_t C1R2, size_t C2>
    matrix_t<decltype(std::declval<T1>() * std::declval<T2>()), R1, C2> matMul(matrix_t<T1, R1, C1R2>& m1, matrix_t<T2, C1R2, C2>& m2)
    {
		matrix_t<decltype(std::declval<T1>() * std::declval<T2>()), R1, C2> r;
        for (size_t i = 0; i < R1; i++)
        {
            for (size_t j = 0; j < C2; j++)
            {
                for (size_t k = 0; k < C1R2; k++)
                {
                   r[i][j] += m1[i][k] * m2[k][j];
                }
            }
        }
        return r;
    }

	template<class T, size_t R, size_t C>
	std::ostream& operator<<(std::ostream& os, const matrix_t<T, R, C>& m)
	{
		return os << m.stringify();
	}
};
