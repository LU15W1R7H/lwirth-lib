#pragma once

#include "Standard.hpp"

#include "Vulkan.hpp"

namespace lw
{
	LWAPI extern void init();
	LWAPI extern void terminate();

	class LWAPI Root
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

