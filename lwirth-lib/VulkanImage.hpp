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
			
			static void s_create(Device* device, U32 width, U32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkImage* image, VkDeviceMemory* imageMemory, U32 mipLevels = 1);
			static void s_createView(Device* device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView* imageView, U32 mipLevels = 1);
			static void s_transitionLayout(const Device* pDevice, const CommandPool* pCommandPool, const Queue* pQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, U32 baseMipLevel = 0, U32 levelCount = 1);
		};

	}
}