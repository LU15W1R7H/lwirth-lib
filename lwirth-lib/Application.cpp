#include "stdafx.hpp"
#include "Application.hpp"

void lw::Application::loop()
{
	while (true)
	{
		m_singleFrameAllocator.clear();

		//only use this frame
		//void* p = m_singleFrameAllocator.alloc(nBytes);
	}
}
