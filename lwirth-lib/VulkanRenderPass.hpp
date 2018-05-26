#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

namespace lw
{
	namespace VK
	{
		class Device;

		class LWAPI RenderPass
		{
		private:
			VkRenderPass	m_renderPass	= VK_NULL_HANDLE;

			Device*			m_pDevice		= nullptr;

		public:
			RenderPass() = default;
			~RenderPass();

			void create(Device* pDevice);
			void destroy();

			VkRenderPass raw() const;
			VkRenderPass* ptr();
		};

	}
}