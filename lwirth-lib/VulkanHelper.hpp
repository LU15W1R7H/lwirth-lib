#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "List.hpp"
#include <set>

namespace lw
{
	namespace VK
	{

		class PhysicalDevice;
		class Device;
		class CommandPool;
		class QueueFamilyIndices;

		LWAPI extern bool isFormatSupported(const PhysicalDevice* physicalDevice, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);
		LWAPI extern VkFormat findSupportedFormat(const PhysicalDevice* physicalDevice, lw::List<VkFormat>* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		LWAPI extern bool hasStencilComponent(VkFormat format);
		LWAPI extern u32 findMemoryTypeIndex(const PhysicalDevice* physicalDevice, u32 typeFilter, VkMemoryPropertyFlags properties);
	}
}

