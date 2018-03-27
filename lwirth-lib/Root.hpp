#pragma once

#include "Standard.hpp"

#include "Vulkan.hpp"

namespace lw
{
	API extern void init();
	API extern void terminate();

	class API Root
	{
		extern friend void init();
		extern friend void terminate();
	private:

	public:

		
	private:

		void init();
		void terminate();

	public:
		static Root& get();
	};
}

