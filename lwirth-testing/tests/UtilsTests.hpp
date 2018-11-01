//
// Created by Luis on 26-Oct-18.
//

#ifndef LWIRTH_UTILSTESTS_HPP
#define LWIRTH_UTILSTESTS_HPP


#include <lwirth.hpp>

lw::Random random;

using Age = lw::StrongType<int, struct Agef>;

void strongTypeTest()
{
    Age age(5);


}

void chronometerTest()
{

}

void fillTest()
{
    lw::List<int> list(1000);
    lw::fill(std::begin(list), std::end(list), [&](){return random.nextI32(800);});

}

void listFillTest()
{
    lw::List<int> list;
    list.fillConstruct(800, [&](){return random.nextI32(67);});
    lw::println(list.size());
}



#endif //LWIRTH_UTILSTESTS_HPP
