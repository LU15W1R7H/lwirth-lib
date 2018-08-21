#pragma once

#include "../Standard.hpp"

#include "../graphics/Vulkan.hpp"

namespace lw
{
	extern void init();
	extern void terminate();

	class Root
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

