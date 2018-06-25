#include "lwirth.hpp"

//#include "GFX.hpp"

using namespace lw;

#include <iostream>



int main()
{
    Vec3 v1(1, 2, 3);
    Vec3 v2(3, 2, 1);
    auto v3 = cross(v1, v2);
    std::cout << v3 << '\n';

	std::cin.get();
}