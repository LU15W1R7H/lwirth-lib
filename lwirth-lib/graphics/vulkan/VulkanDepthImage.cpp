#include "../../stdafx.hpp"
#include "VulkanDepthImage.hpp"

#include "../../general/Exceptions.hpp"
#include "VulkanImage.hpp"
#include "VulkanHelper.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "../../memory/List.hpp"

namespace lw
{
	namespace VK
	{

		DepthImage::~DepthImage()
		{
			if (m_image != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void DepthImage::create(Device* pDevice, CommandPool* pCommandPool, u32 width, u32 height)
		{
			if (m_image != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;

			VkFormat depthFormat = findDepthFormat(pDevice->getPhysical());

			VkImageCreateInfo ici;
			ici.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
			ici.pNext = nullptr;
			ici.flags = 0;
			ici.imageType = VK_IMAGE_TYPE_2D;
			ici.format = depthFormat;
			ici.extent.width = width;
			ici.extent.height = height;
			ici.extent.depth = 1;
			ici.mipLevels = 1;
			ici.arrayLayers = 1;
			ici.samples = VK_SAMPLE_COUNT_1_BIT;
			ici.tiling = VK_IMAGE_TILING_OPTIMAL;
			ici.usage = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
			ici.sharingMode = VK_SHARING_MODE_EXCLUSIVE;
			ici.queueFamilyIndexCount = 0;
			ici.pQueueFamilyIndices = nullptr;
			ici.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;

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
			ivci.format = depthFormat;
			ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
			ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_DEPTH_BIT;
			ivci.subresourceRange.baseMipLevel = 0;
			ivci.subresourceRange.levelCount = 1;
			ivci.subresourceRange.baseArrayLayer = 0;
			ivci.subresourceRange.layerCount = 1;

			if (vkCreateImageView(m_pDevice->raw(), &ivci, nullptr, &m_view) != VK_SUCCESS)
			{
				throw VulkanException("failed to create image view");
			}
		
			//transition layout
			transitionImageLayout(*m_pDevice, *pCommandPool, m_image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
		}

		void DepthImage::destroy()
		{
			if (m_image == VK_NULL_HANDLE)return;
			vkDestroyImageView(m_pDevice->raw(), m_view, nullptr);
			m_view = VK_NULL_HANDLE;
			vkDestroyImage(m_pDevice->raw(), m_image, nullptr);
			m_image = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;

			m_pDevice = nullptr;
		}

		VkImage DepthImage::image() const
		{
			if (m_image == VK_NULL_HANDLE)throw NotCreatedException();
			return m_image;
		}

		VkDeviceMemory DepthImage::memory() const
		{
			if (m_image == VK_NULL_HANDLE)throw NotCreatedException();
			return m_memory;
		}

		VkImageView DepthImage::view() const
		{
			if (m_image == VK_NULL_HANDLE)throw NotCreatedException();
			return m_view;
		}


		VkFormat DepthImage::findDepthFormat(PhysicalDevice * physicalDevice)
		{
			lw::List<VkFormat> formats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

			return findSupportedFormat(physicalDevice, &formats, VK_IMAGE_TILING_OPTIMAL, VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT);
		}

		VkAttachmentDescription DepthImage::getDepthAttachment(PhysicalDevice* physicalDevice)
		{
			VkAttachmentDescription depthAttachment;
			depthAttachment.flags = 0;
			depthAttachment.format = findDepthFormat(physicalDevice);
			depthAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			depthAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			depthAttachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			depthAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			depthAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			depthAttachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
			
			return depthAttachment;
		}
	}
}