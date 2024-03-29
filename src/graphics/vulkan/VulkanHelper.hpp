#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../../memory/List.hpp"
#include <set>

namespace lw
{
	namespace VK
	{

		class PhysicalDevice;
		class Device;
		class CommandPool;
		class QueueFamilyIndices;

		extern bool isFormatSupported(const PhysicalDevice* physicalDevice, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features);
		extern VkFormat findSupportedFormat(const PhysicalDevice* physicalDevice, lw::List<VkFormat>* candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
		extern bool hasStencilComponent(VkFormat format);
		extern u32 findMemoryTypeIndex(const PhysicalDevice* physicalDevice, u32 typeFilter, VkMemoryPropertyFlags properties);
	}
}

