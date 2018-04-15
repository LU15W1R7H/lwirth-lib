#include "stdafx.hpp"
#include "VulkanHelper.hpp"

#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	namespace VK
	{

		bool isFormatSupported(const PhysicalDevice* physicalDevice, VkFormat format, VkImageTiling tiling, VkFormatFeatureFlags features)
		{
			VkFormatProperties props;
			vkGetPhysicalDeviceFormatProperties(physicalDevice->raw(), format, &props);

			if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features)
			{
				return true;
			}
			else if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features)
			{
				return true;
			}

			return false;
		}

		VkFormat findSupportedFormat(const PhysicalDevice* physicalDevice, lw::DynamicArray<VkFormat>* candidates, VkImageTiling tiling, VkFormatFeatureFlags features)
		{
			for (const VkFormat& format : *candidates)
			{
				if (isFormatSupported(physicalDevice, format, tiling, features))
				{
					return format;
				}
			}

			throw VulkanException("failed to find supported format");
		}

		bool hasStencilComponent(VkFormat format)
		{
			return format == VK_FORMAT_D32_SFLOAT_S8_UINT || format == VK_FORMAT_D24_UNORM_S8_UINT;
		}


		u32 findMemoryTypeIndex(const PhysicalDevice* physicalDevice, u32 typeFilter, VkMemoryPropertyFlags properties)
		{
			VkPhysicalDeviceMemoryProperties pdmp;
			vkGetPhysicalDeviceMemoryProperties(physicalDevice->raw(), &pdmp);

			for (u32 i = 0; i < pdmp.memoryTypeCount; i++)
			{
				if (typeFilter & (1 << i) && (pdmp.memoryTypes[i].propertyFlags & properties) == properties)
				{
					return i;
				}
			}

			throw VulkanException("failed to find suitable memory type");
		}

		

	}
}