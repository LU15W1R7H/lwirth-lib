#include "lwirth.hpp"

#include "tests/VulkanTest.hpp"


int main()
{

	lw::List<int> list = {3, 5, 2, 76, 34};

	for(const auto& e : list)
	{
		std::cout << e << '\n';
	}

	std::cin.get();

	//return vulkanTest();
}
