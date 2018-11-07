#pragma once

#include "Standard.hpp"

#include <iostream>

namespace lw
{

	template<class Only>
	void print(Only&& only)
	{
		std::cout << only;
	}

	template<class First, class... Rest>
	void print(First&& first, Rest&&... rest)
	{
		std::cout << first;
		print(rest...);
	}

	template<class Only>
	void println(Only&& only)
	{
		std::cout << only << '\n';
	}

	template<class First, class... Rest>
	void println(First&& first, Rest&&... rest)
	{
		std::cout << first;
		println(rest...);
	}

}



