#include <lwirth.hpp>

int main()
{

	lw::List<float> list = {3, 5, 2, 76, 34};
	list.push(69, 33, 8.2f, 2.f);

	//lw::println(list.size());

	//list.push(3, 5, 2);
	
	for(const auto& e : list)
	{
		lw::println(e);
	}

	std::cin.get();

	//return vulkanTest();
}
