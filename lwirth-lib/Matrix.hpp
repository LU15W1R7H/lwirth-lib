#pragma once

#include "Standard.hpp"

#include <type_traits>
#include "Vector.hpp"

namespace lw
{
	template<class T, size_t R, size_t C>
    class elements_t
    {
    public:
        using Type = T;

        std::array<std::array<T, C>, R> elems;

        constexpr static size_t getRows()
        {
            return R;
        }

        constexpr static size_t getColumns()
        {
            return C;
        }

        constexpr static size_t getSize()
        {
            return R * C;
        }
    };

    template<class ELEMENTS>
    class matrix_base : public ELEMENTS
    {
    public:
        using SELF = matrix_base<ELEMENTS>;

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

        SELF& operator=(SELF&& m)
        {
            ELEMENTS::operator=(m);
            return *this;
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

        ELEMENTS::Type det() const
        {
            //return determinant
        }

        

    };

    //standard matrix
    template<class T, size_t R, size_t C>
    class matrix_t : public matrix_base<elements_t<T, R, C>>
    {

    };

    //square matrix
    template<class T, size_t S>
    class matrix_t<T, S, S> : public matrix_base<elements_t<T, S, S>
    {

    }
};
