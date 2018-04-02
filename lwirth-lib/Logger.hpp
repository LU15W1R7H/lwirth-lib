#pragma once

#include "Standard.hpp"

#include <iostream>

namespace lw
{

	template<typename T>
	void print(T msg)
	{
		std::cout << msg;
	}

	template<typename T>
	void println(T msg)
	{
		std::cout << msg << '\n';
	}

	class Logger
	{
	public:

		
	};
}



