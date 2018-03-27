#include "stdafx.hpp"
#include "VulkanImage.hpp"

#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"
#include "VulkanHelper.hpp"
#include "VulkanQueues.hpp"

namespace lw
{
	namespace VK
	{
		void Image::s_create(Device * device, U32 width, U32 height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usageFlags, VkMemoryPropertyFlags propertyFlags, VkImage * image, VkDeviceMemory * imageMemory, U32 mipLevels)
		{
			VkImageCreateInfo ici;
			ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			ici.pNext = nullptr;;
			ici.flags = 0;
			ici.imageType = VK_IMAGE_TYPE_2D;
			ici.format = format;
			ici.extent.width = width;
			ici.extent.height = height;
			ici.extent.depth = 1;
			ici.mipLevels = mipLevels;
			ici.arrayLayers = 1;
			ici.samples = VK_SAMPLE_COUNT_1_BIT;
			ici.tiling = tiling;
			ici.usage = usageFlags;
			ici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			ici.queueFamilyIndexCount = 0;
			ici.pQueueFamilyIndices = nullptr;
			ici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

			if (vkCreateImage(device->raw(), &ici, nullptr, image) != VK_SUCCESS)
			{
				throw VulkanException("failed to create image");
			}

			VkMemoryRequirements mr;
			vkGetImageMemoryRequirements(device->raw(), *image, &mr);

			VkMemoryAllocateInfo mai;
			mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mai.pNext = nullptr;
			mai.allocationSize = mr.size;
			mai.memoryTypeIndex = findMemoryTypeIndex(device->getPhysical(), mr.memoryTypeBits, propertyFlags);

			if (vkAllocateMemory(device->raw(), &mai, nullptr, imageMemory) != VK_SUCCESS)
			{
				throw VulkanException("failed to allocate image memory");
			}

			vkBindImageMemory(device->raw(), *image, *imageMemory, 0);
		}

		void Image::s_createView(Device * device, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, VkImageView * imageView, U32 mipLevels)
		{
			VkImageViewCreateInfo ivci;
			ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ivci.pNext = nullptr;
			ivci.flags = 0;
			ivci.image = image;
			ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ivci.format = format;
			ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.subresourceRange.aspectMask = aspectFlags;
			ivci.subresourceRange.baseMipLevel = 0;
			ivci.subresourceRange.levelCount = 1;
			ivci.subresourceRange.baseArrayLayer = 0;
			ivci.subresourceRange.layerCount = 1;

			if (vkCreateImageView(device->raw(), &ivci, nullptr, imageView) != VK_SUCCESS)
			{
				throw VulkanException("failed to create texture image");
			}
		}

		void Image::s_transitionLayout(const Device* pDevice, const CommandPool* pCommandPool, const Queue* pQueue, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, U32 baseMipLevel, U32 levelCount)
		{
			CommandBuffer commandBuffer;
			commandBuffer.allocate(pDevice, pCommandPool);
			commandBuffer.beginSingleTime();

			VkImageMemoryBarrier imb;
			imb.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
			imb.pNext = nullptr;

			VkPipelineStageFlags srcStage;
			VkPipelineStageFlags dstStage;

			if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				imb.srcAccessMask = 0;
				imb.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				imb.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imb.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL)
			{
				imb.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imb.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

				srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_UNDEFINED && newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				imb.srcAccessMask = 0;
				imb.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

				srcStage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
				dstStage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL)
			{
				imb.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
				imb.dstAccessMask = VK_ACCESS_TRANSFER_READ_BIT;

				srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else if (oldLayout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && newLayout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL)
			{
				imb.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
				imb.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

				srcStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
				dstStage = VK_PIPELINE_STAGE_TRANSFER_BIT;
			}
			else
			{
				throw std::invalid_argument("layout transition is not supported");
			}

			imb.oldLayout = oldLayout;
			imb.newLayout = newLayout;
			imb.srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imb.dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED;
			imb.image = image;

			if (newLayout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL)
			{
				imb.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
				if (hasStencilComponent(format))
				{
					imb.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
				}
			}
			else
			{
				imb.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			}

			imb.subresourceRange.baseMipLevel = baseMipLevel;
			imb.subresourceRange.levelCount = levelCount;
			imb.subresourceRange.baseArrayLayer = 0;
			imb.subresourceRange.layerCount = 1;

			vkCmdPipelineBarrier(commandBuffer.raw(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &imb); //#TODO check for stage

			commandBuffer.endAndExecuteAndFree(pQueue);
		}
	}
}
