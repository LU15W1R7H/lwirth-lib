#include "lwirth.hpp"

#include "GFX.hpp"

using namespace lw;



int main()
{
	//vk();

    lw::vector_t<5, f32> vec;
    vec[0] = 1;
    vec[1] = 12;
    vec[2] = 23;
    vec[3] = 56;
    vec[4] = 542;
    auto string = vec.stringify();
    std::cout << vec << '\n';

	std::cin.get();
}