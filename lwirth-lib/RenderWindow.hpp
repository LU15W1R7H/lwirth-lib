#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Frame.hpp"
#include "VulkanManager.hpp"
#include <string>

namespace lw
{
	class API RenderWindow : public Frame
	{
	private:
		VK::VulkanManager m_vulkan;
	public:
		void create(std::string& title, U32 width, U32 height) override;
		void create(std::string&& title, U32 width, U32 height) override;
		void destroy() override;

		void update() override;

		Frame* getFrame() { return static_cast<Frame*>(this); }

		SimpleBrush2D* preDraw();
		void postDraw();

	private:
		void resize(U32 width, U32 height) override;
	};

}