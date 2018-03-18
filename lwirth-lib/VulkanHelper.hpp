#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <vector>
#include <set>

namespace lw
{
	namespace VK
	{
		class PhysicalDevice;
		class Device;
		class CommandPool;
		class QueueFamilyIndices;

		API extern bool isFormatSupported(const PhysicalDevice* physicalDevice, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);
		API extern VkFormat findSupportedFormat(const PhysicalDevice* physicalDevice, std::vector<VkFormat>* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		API extern bool hasStencilComponent(VkFormat format);
		API extern U32 findMemoryTypeIndex(const PhysicalDevice* physicalDevice, U32 typeFilter, VkMemoryPropertyFlags properties);
	}
}

