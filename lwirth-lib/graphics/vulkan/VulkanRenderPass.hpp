#pragma once

#include "../../Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "../../memory/List.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class CommandBuffer;

		class RenderPass
		{
		private:
			VkRenderPass	m_renderPass	= VK_NULL_HANDLE;

			Device*			m_pDevice		= nullptr;

		public:
			RenderPass() = default;
			~RenderPass();

			void create(Device* pDevice);
			void destroy();

			void begin(const CommandBuffer& cmd, VkFramebuffer framebuffer, VkRect2D renderArea, const List<VkClearValue>& clearValues, VkSubpassContents contents);

			VkRenderPass raw() const;
			VkRenderPass* ptr();
		};

	}
}