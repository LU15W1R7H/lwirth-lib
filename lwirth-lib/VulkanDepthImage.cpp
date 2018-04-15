#include "stdafx.hpp"
#include "VulkanDepthImage.hpp"

#include "Exceptions.hpp"
#include "VulkanImage.hpp"
#include "VulkanHelper.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "DynamicArray.hpp"

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

			VkFormat depthFormat = findDepthFormat(m_pDevice->getPhysical());

			Image::s_create(m_pDevice, width, height, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, &m_image, &m_memory);
			Image::s_createView(m_pDevice, m_image, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, &m_view);
			Image::s_transitionLayout(m_pDevice, pCommandPool, pDevice->getGraphicsQueue(), m_image, depthFormat, VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL);
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
			lw::DynamicArray<VkFormat> formats = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };

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