#pragma once

#include "../Standard.hpp"

#include "../memory/StackAllocator.hpp"

namespace lw
{
	class Application
	{
	private:
		StackAllocator<> m_singleFrameAllocator;


	public:

	private:
		void loop();
	};
}