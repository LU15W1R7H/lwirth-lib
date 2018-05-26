#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Frame.hpp"
#include "Vulkan.hpp"
#include <string>

namespace lw
{
	class RenderWindow : public Frame
	{
	private:
		VK::Vulkan m_vulkan;
	public:
		void create(std::string& title, u32 width, u32 height) override;
		void create(std::string&& title, u32 width, u32 height) override;
		void destroy() override;

		void update() override;

		Frame* getFrame();
		VK::Vulkan* getVulkan();

		void preDraw();
		void postDraw();

	private:
		void resize(u32 width, u32 height) override;
	};

}