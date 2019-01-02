#include "RenderWindow.hpp"

#include "../general/Exceptions.hpp"

#include <string>
#include <iostream>

namespace lw
{
	void RenderWindow::create(std::string& title, u32 width, u32 height)
	{
		if (m_created)throw AlreadyCreatedException();

		Frame::create(title, width, height);

		m_vulkan.start(getFrame());
	}

	void RenderWindow::create(std::string&& title, u32 width, u32 height)
	{
		RenderWindow::create(title, width, height);
	}
	
	void RenderWindow::destroy()
	{
		if (m_created)
		{
			m_vulkan.destroy();

			Frame::destroy();
		}
	}

	void RenderWindow::update()
	{
		Frame::update();
	}

	Frame* RenderWindow::getFrame()
	{
		if (!m_created)throw NotCreatedException();
		return static_cast<Frame*>(this);
	}

	VK::Vulkan* RenderWindow::getVulkan()
	{
		if (!m_created)throw NotCreatedException();
		return &m_vulkan;
	}

	void RenderWindow::preDraw()
	{
		m_vulkan.preDraw();
	}

	void RenderWindow::postDraw()
	{
		m_vulkan.postDraw();
	}

	void RenderWindow::resize(u32 width, u32 height)
	{
		m_vulkan.resize(width, height);
	}

}
