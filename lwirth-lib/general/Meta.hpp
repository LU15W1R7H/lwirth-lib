#pragma once

#include "../Standard.hpp"

namespace lw
{
    namespace DETAIL
    {
        template<class T, size_t N, size_t POS = 0>
        struct assign_helper
        {
            template<class FIRST, class ... REST>
            static void assign(T (&arr)[N], FIRST&& first, REST&& ... rest)
            {
                arr[POS] = std::forward<FIRST>(first);
                assign_helper<T, N, POS + 1>::assign(arr, std::forward<REST>(rest)...);
            }
            static void assign(T (&arr)[N]) {}
        };

        template<class T, size_t N>
        struct assign_helper<T, N, N> //->array is out of range
        {
            template<class ... REST>
            static void assign(T (&arr)[N], const REST& ...) {}
        };
    }

    template<class T, size_t N, class ... ARGS>
    void assign(T (&arr)[N], ARGS&& ... args)
    {
        static_assert(sizeof...(args) >= N, "too few arguments to assign");
        DETAIL::assign_helper<T, N, 0>::assign(arr, std::forward<ARGS>(args)...);
    }
}