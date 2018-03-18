#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

namespace lw
{
	namespace VK
	{
		class Device;

		class API RenderPass
		{
		private:
			VkRenderPass	m_renderPass	= VK_NULL_HANDLE;

			Device*			m_pDevice		= nullptr;

		public:
			RenderPass();
			~RenderPass();

			void create(Device* pDevice);
			void destroy();

			VkRenderPass raw() const;
			VkRenderPass* ptr();
		};

	}
}