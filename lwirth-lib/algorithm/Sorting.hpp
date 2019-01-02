#include <algorithm>

/*
 * IMPORTANT NOTE:
 * Make sure the comparison-condition CAN be met.
 * Especially if multiple identical elements exist.
 * Otherwise the function will never return and run forever.
 * E.g. the condition [a < b] can't be satisfied if two identical elements exist.
 */

namespace lw
{

    template<typename ForwardIter, typename Condition>
    void bubbleSort(ForwardIter begin, ForwardIter end, Condition comparison)
    {
        bool swaped;
        do
        {
            swaped = false;
            for(auto iter = begin; iter != end - 1; iter++)
            {
                if(!comparison(iter[0], iter[1]))
                {
                  std::swap(iter[0], iter[1]);
                  swaped = true;
                }
            }
        }
        while(swaped);
        
    }
    
    namespace detail
    {
        
        template<typename ForwardIter, typename Condition>
        ForwardIter seperate(ForwardIter first, ForwardIter last, Condition comparison)
        {
            lw::print(std::distance(first, last), " ");
            auto pivot = first + std::distance(first, last)/2;
            auto i = first - 1;
            auto j = last + 1;
            
            while(true)
            {
                do
                {
                    i++;
                }
                while(comparison(*i, *pivot));

                do
                {
                    j--;
                }
                while(comparison(*pivot, *j));

                if(i >= j)
                {
                    return j;
                }

                std::swap(*i, *j);
            }
        }
        template<typename ForwardIter, typename Condition>
        void quickSort(ForwardIter first, ForwardIter last, Condition comparison)
        {
            if(first < last)
            {
                auto sep = detail::seperate(first, last, comparison);
                detail::quickSort(first, sep, comparison);
                detail::quickSort(sep+1, last, comparison);
            }
        }

    } //namespace detail

    
    template<typename ForwardIter, typename Condition>
    void quickSort(ForwardIter begin, ForwardIter end, Condition comparison)
    {
        detail::quickSort(begin, end - 1, comparison);
    }
    
} //namespace lw
