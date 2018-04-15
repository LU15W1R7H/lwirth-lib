#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

namespace lw
{
	namespace VK
	{
		class Device;
		class PhysicalDevice;
		class CommandPool;

		class API DepthImage
		{
		private:
			VkImage			m_image		= VK_NULL_HANDLE;
			VkDeviceMemory	m_memory	= VK_NULL_HANDLE;
			VkImageView		m_view		= VK_NULL_HANDLE;

			Device*			m_pDevice	= nullptr;

		public:
			DepthImage() = default;
			~DepthImage();

			void create(Device* pDevice, CommandPool* pCommandPool, u32 width, u32 height);
			void destroy();

			VkImage image() const;
			VkDeviceMemory memory() const;
			VkImageView view() const;

			static VkFormat findDepthFormat(PhysicalDevice* physicalDevice);
			static VkAttachmentDescription getDepthAttachment(PhysicalDevice* physicalDevice);
		};

	}
}