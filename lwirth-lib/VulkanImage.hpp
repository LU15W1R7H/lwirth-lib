#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

namespace lw
{
	namespace VK
	{
		class Device;
		class CommandPool;
		class Queue;

		class API Image
		{
		private:

		public:
			
			static void s_create(Device* device, u32 width, u32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkImage* image, VkDeviceMemory* imageMemory, u32 mipLevels = 1);
			static void s_createView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView, u32 mipLevels = 1);
			static void s_transitionLayout(const Device* pDevice, const CommandPool* pCommandPool, const Queue* pQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, u32 baseMipLevel = 0, u32 levelCount = 1);
		};

	}
}