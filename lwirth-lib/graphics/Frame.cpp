#include "../stdafx.hpp"
#include "Frame.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../general/Exceptions.hpp"
#include <iostream>

namespace lw
{
	size_t Frame::s_frameCount = 0;

	Frame::Frame()
	{
	}

	Frame::~Frame()
	{
		Frame::destroy();
	}

	void Frame::create(std::string& title, u32 width, u32 height)
	{
		if (m_created)throw AlreadyCreatedException();

		if (s_frameCount == 0)
		{
			glfwInit();
			glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		}

		//SECONDARY MONITOR -> DELETE
		int monitorCount;
		GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
		int x, y;
		glfwGetMonitorPos(monitors[monitorCount - 1], &x, &y);
		const GLFWvidmode* vidmode = glfwGetVideoMode(monitors[monitorCount - 1]);
		x += vidmode->width / 2 - width / 2;
		y += vidmode->height / 2 - height / 2;

		m_pFrame = glfwCreateWindow(width, height, title.c_str(), nullptr, nullptr);

		glfwSetWindowUserPointer(m_pFrame, this);
		glfwSetWindowSizeCallback(m_pFrame, Frame::resizeCallback);
		glfwSetKeyCallback(m_pFrame, Frame::keyCallback);
		glfwSetMouseButtonCallback(m_pFrame, Frame::mouseButtonCallback);
		glfwSetCursorPosCallback(m_pFrame, Frame::cursorPositionCallback);
		glfwSetWindowPos(m_pFrame, x, y);

		m_created = true;
		s_frameCount++;

	}

	void Frame::create(std::string&& title, u32 width, u32 height)
	{
		Frame::create(title, width, height);
	}

	void Frame::destroy()
	{
		if (m_created)
		{
			glfwDestroyWindow(m_pFrame);

			if (s_frameCount == 1)glfwTerminate();

			m_created = false;
			s_frameCount--;
		}
	}

	void Frame::update()
	{
		glfwPollEvents();
	}

	bool Frame::isOpen() const
	{
		return !glfwWindowShouldClose(m_pFrame);
	}

	void Frame::open() const
	{
		glfwSetWindowShouldClose(m_pFrame, GLFW_FALSE);
	}

	void Frame::close() const
	{
		glfwSetWindowShouldClose(m_pFrame, GLFW_TRUE);
	}

	u32 Frame::getWidth() const
	{
		int x, y;
		glfwGetWindowSize(m_pFrame, &x, &y);
		return static_cast<u32>(x);
	}

	u32 Frame::getHeight() const
	{
		int x, y;
		glfwGetWindowSize(m_pFrame, &x, &y);
		return static_cast<u32>(y);
	}

	void Frame::setWidth(u32 newWidth) const
	{
		glfwSetWindowSize(m_pFrame, newWidth, getHeight());
	}

	void Frame::setHeight(u32 newHeight) const
	{
		glfwSetWindowSize(m_pFrame, getWidth(), newHeight);
	}

	void Frame::setSize(u32 newWidth, u32 newHeight) const
	{
		glfwSetWindowSize(m_pFrame, newWidth, newHeight);
	}

	u32 Frame::getPosX() const
	{
		int x, y;
		glfwGetWindowPos(m_pFrame, &x, &y);
		return x;
	}

	u32 Frame::getPosY() const
	{
		int x, y;
		glfwGetWindowPos(m_pFrame, &x, &y);
		return y;
	}

	void Frame::setPosX(u32 newX) const
	{
		glfwSetWindowPos(m_pFrame, newX, getPosY());
	}

	void Frame::setPosY(u32 newY) const
	{
		glfwSetWindowPos(m_pFrame, newY, getPosY());
	}

	void Frame::setPos(u32 newX, u32 newY) const
	{
		glfwSetWindowPos(m_pFrame, newX, newY);
	}

	void Frame::setTitle(std::string& newTitle) const
	{
		glfwSetWindowTitle(m_pFrame, newTitle.c_str());
	}

	void Frame::setTitle(std::string&& newTitle) const
	{
		glfwSetWindowTitle(m_pFrame, newTitle.c_str());
	}

	void Frame::setMinimized(bool enable) const
	{
		if (enable)
			glfwIconifyWindow(m_pFrame);
		else
			glfwRestoreWindow(m_pFrame);
	}

	void Frame::setVisibility(bool enable) const
	{
		if (enable)
			glfwShowWindow(m_pFrame);
		else
			glfwHideWindow(m_pFrame);
	}

	void Frame::setFocus() const
	{
		glfwFocusWindow(m_pFrame);
	}

	void Frame::keyCallback(GLFWwindow* pFrame, int key, int scancode, int action, int mods)
	{
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(pFrame));

		if (action != GLFW_PRESS)
		{
			return;
		}

		std::cout << "Key: " << key << std::endl;

		switch (key)
		{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(pFrame, GLFW_TRUE);
		}
	}

	void Frame::mouseButtonCallback(GLFWwindow* pFrame, int button, int action, int mods)
	{
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(pFrame));

		//std::cout << "Mouse Button: " << button << std::endl;
	}

	void Frame::cursorPositionCallback(GLFWwindow* pFrame, double xpos, double ypos)
	{
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(pFrame));

		//std::cout << "Cursor X: " << xpos << " Y: " << ypos << std::endl;
	}

	void Frame::resizeCallback(GLFWwindow* pFrame, int width, int height)
	{
		Frame* frame = reinterpret_cast<Frame*>(glfwGetWindowUserPointer(pFrame));
		
		frame->resize(width, height);
	}
}