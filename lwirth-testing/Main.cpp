#include <lwirth.hpp>

int main()
{

	lw::List<int> list = {3, 5, 2, 76, 34};

	for(const auto& e : list)
	{
		lw::print(e);
	}

	std::cin.get();

	//return vulkanTest();
}
