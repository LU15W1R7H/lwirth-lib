#include "stdafx.hpp"
#include "VulkanImage.hpp"

#include "Logger.hpp"

#include "Exceptions.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"
#include "VulkanHelper.hpp"
#include "VulkanQueues.hpp"
#include "VulkanBuffer.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include "../libraries/stb/stb_image.h"

namespace lw
{
	namespace VK
	{

		void transitionImageLayout(const Device & device, const CommandPool & cmdPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout)
		{
			CommandBuffer commandBuffer;
			commandBuffer.allocate(&device, &cmdPool);
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

			imb.subresourceRange.baseMipLevel = 0;
			imb.subresourceRange.levelCount = 1;
			imb.subresourceRange.baseArrayLayer = 0;
			imb.subresourceRange.layerCount = 1;

			vkCmdPipelineBarrier(commandBuffer.raw(), srcStage, dstStage, 0, 0, nullptr, 0, nullptr, 1, &imb); //#TODO check for stage

			commandBuffer.endAndExecuteAndFree(device.getGraphicsQueue());
		}

		void Image::create(const VK::Device& device, u32 width, u32 height, VkFormat format, VkImageUsageFlags usage)
		{
			m_pDevice = &device;

			VkImageCreateInfo ici;
			ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			ici.pNext = nullptr;
			ici.flags = 0;
			ici.imageType = VK_IMAGE_TYPE_2D; //could also be 1D or 3D
			ici.format = (VkFormat)format;
			ici.extent.width = width;
			ici.extent.height = height;
			ici.extent.depth = 1;
			ici.mipLevels = 1;
			ici.arrayLayers = 1;
			ici.samples = VK_SAMPLE_COUNT_1_BIT;
			ici.tiling = VK_IMAGE_TILING_OPTIMAL; //should be linear for StagingImage
			ici.usage = usage; //must be versatile
			ici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			ici.queueFamilyIndexCount = 0;
			ici.pQueueFamilyIndices = nullptr;
			ici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED; //UNDEFINED discards texels and PREINITALIZED preserves them, but neither of them are usable by the GPU
			m_layout = VK_IMAGE_LAYOUT_UNDEFINED;

			if (vkCreateImage(m_pDevice->raw(), &ici, nullptr, &m_image) != VK_SUCCESS)
			{
				throw VulkanException("failed to create image");
			}

			VkMemoryRequirements memReq;
			vkGetImageMemoryRequirements(m_pDevice->raw(), m_image, &memReq);


			VkMemoryAllocateInfo alloc;
			alloc.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			alloc.pNext = nullptr;
			alloc.allocationSize = memReq.size;
			alloc.memoryTypeIndex = findMemoryTypeIndex(m_pDevice->getPhysical(), memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

			if (vkAllocateMemory(m_pDevice->raw(), &alloc, nullptr, &m_memory) != VK_NULL_HANDLE)
			{
				throw VulkanException("failed to allocate image memory");
			}

			vkBindImageMemory(m_pDevice->raw(), m_image, m_memory, 0);

			//create image view
			VkImageViewCreateInfo ivci;
			ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
			ivci.pNext = nullptr;
			ivci.flags = 0;
			ivci.image = m_image;
			ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
			ivci.format = (VkFormat)format;
			ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			ivci.subresourceRange.baseMipLevel = 0;
			ivci.subresourceRange.levelCount = 1;
			ivci.subresourceRange.baseArrayLayer = 0;
			ivci.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_pDevice->raw(), &ivci, nullptr, &m_view) != VK_SUCCESS)
			{
				throw VulkanException("failed to create image view");
			}

			//create sampler
			VkSamplerCreateInfo sci;
			sci.sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO;
			sci.pNext = nullptr;
			sci.flags = 0;
			sci.magFilter = VK_FILTER_LINEAR; //variable
			sci.minFilter = VK_FILTER_LINEAR;
			sci.mipmapMode = VK_SAMPLER_MIPMAP_MODE_LINEAR;
			sci.addressModeU = m_repeatMode;
			sci.addressModeV = m_repeatMode;
			sci.addressModeW = m_repeatMode;
			sci.mipLodBias = 0.f;
			sci.anisotropyEnable = VK_TRUE;
			sci.maxAnisotropy = 16;
			sci.compareEnable = VK_FALSE;
			sci.compareOp = VK_COMPARE_OP_ALWAYS;
			sci.minLod = 0.f;
			sci.maxLod = 0.f;
			sci.borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK;
			sci.unnormalizedCoordinates = VK_FALSE;

			if (vkCreateSampler(m_pDevice->raw(), &sci, nullptr, &m_sampler) != VK_SUCCESS)
			{
				throw VulkanException("failed to create image sampler");
			}
		}

		void Image::createAndLoadFromFile(const VK::Device & device, const CommandPool & cmdPool, const std::string & path)
		{

			int width, height, channels;
			stbi_uc* pixels = stbi_load(path.c_str(), &width, &height, &channels, STBI_rgb_alpha);

			VkDeviceSize imageSize = width * height * 4;

			if (!pixels)
			{
				throw lw::VulkanException("failed to load image");
			}

			StagingBuffer staging;

			staging.allocate(m_pDevice, imageSize);
			void* ptr = staging.map();
			memcpy(ptr, pixels, static_cast<size_t>(imageSize));
			staging.unmap();

			stbi_image_free(pixels);

			Image::createAndCopyFromBuffer(*m_pDevice, cmdPool, width, height, staging);
			staging.destroy();
		}

		void Image::createAndCopyFromBuffer(const VK::Device& device, const CommandPool& cmdPool, u32 width, u32 height, const VK::StagingBuffer& staging)
		{
			Image::create(device, width, height, VK_FORMAT_R8G8B8A8_UNORM, VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_SAMPLED_BIT);

			VkBufferImageCopy bic;
			bic.bufferOffset = 0;
			bic.bufferRowLength = 0;
			bic.bufferImageHeight = 0;
			bic.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
			bic.imageSubresource.mipLevel = 0;
			bic.imageSubresource.baseArrayLayer = 0;
			bic.imageSubresource.layerCount = 1;
			bic.imageOffset = { 0, 0, 0 };
			bic.imageExtent = { width, height, 1 };

			CommandBuffer cmd;
			cmd.allocate(m_pDevice, &cmdPool);
			cmd.beginSingleTime();

			Image::transitionLayout(cmdPool, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);
			vkCmdCopyBufferToImage(cmd.raw(), staging.raw(), m_image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &bic);
			cmd.endAndExecuteAndFree(device.getGraphicsQueue());
			Image::transitionLayout(cmdPool, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
		}

		void Image::destroy()
		{
			if (!exists())return;

			vkDestroySampler(m_pDevice->raw(), m_sampler, nullptr);
			m_sampler = VK_NULL_HANDLE;
			vkDestroyImageView(m_pDevice->raw(), m_view, nullptr);
			m_view = VK_NULL_HANDLE;
			vkDestroyImage(m_pDevice->raw(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;


			m_pDevice = nullptr;
		}

		bool Image::exists() const
		{
			return m_image != VK_NULL_HANDLE;
		}

		VkImage Image::raw() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_image;
		}

		VkDeviceMemory Image::getMemory() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_memory;
		}

		VkImageView Image::getView() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_view;
		}

		VkSampler Image::getSampler() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_sampler;
		}

		u32 Image::getWidth() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_width;
		}

		u32 Image::getHeight() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_height;
		}

		VkFormat Image::getFormat() const
		{
			if (!exists())throw NotCreatedException("Image was not created");
			return m_format;
		}

		void Image::transitionLayout(const CommandPool & cmdPool, VkImageLayout newLayout)
		{
			transitionImageLayout(*m_pDevice, cmdPool, m_image, m_format, m_layout, newLayout);
		}

	}
}
