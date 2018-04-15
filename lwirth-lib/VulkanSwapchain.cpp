#include "stdafx.hpp"
#include "VulkanSwapchain.hpp"

#include "Exceptions.hpp"
#include "Frame.hpp"
#include "VulkanSurface.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanDepthImage.hpp"
#include "Math.hpp"

namespace lw
{
	namespace VK
	{

		void Swapchain::create(Device* pDevice, Surface* pSurface, DepthImage* pDepthImage, RenderPass* pRenderPass, u32 width, u32 height, Swapchain* pOldSwapchain)
		{
			if (m_swapchain != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;

			chooseExtent(width, height);
			chooseImageCount();

			VkSwapchainCreateInfoKHR sci;
			sci.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
			sci.pNext = nullptr;
			sci.flags = 0;
			sci.surface = pSurface->raw();
			sci.minImageCount = m_imageCount;
			sci.imageFormat = m_pDevice->getSurfaceFormat().format;
			sci.imageColorSpace = m_pDevice->getSurfaceFormat().colorSpace;
			sci.imageExtent = m_extent;
			sci.imageArrayLayers = 1;
			sci.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

			auto familyIndices = m_pDevice->getPhysical()->queueFamiles()->getIndices();
			
			if (familyIndices[0] != familyIndices[1])
			{
				sci.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
				sci.queueFamilyIndexCount = familyIndices.size();
				sci.pQueueFamilyIndices = familyIndices.raw();
			}
			else
			{
				sci.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
				sci.queueFamilyIndexCount = 0;
				sci.pQueueFamilyIndices = nullptr;
			}

			sci.preTransform = m_pDevice->getPhysical()->surfaceCapabilities().currentTransform;
			sci.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
			sci.presentMode = m_pDevice->getPresentMode();
			sci.clipped = VK_TRUE;
			sci.oldSwapchain = (pOldSwapchain == nullptr) ? VK_NULL_HANDLE : pOldSwapchain->raw();

			if (vkCreateSwapchainKHR(m_pDevice->raw(), &sci, nullptr, &m_swapchain) != VK_SUCCESS)
			{
				throw VulkanException("failed to create swapchain");
			}

			setupImages();
			setupFramebuffers(pDepthImage, pRenderPass);
		}

		void Swapchain::destroy()
		{
			if (m_swapchain == VK_NULL_HANDLE)return;

			for (u32 i = 0; i < m_imageCount; i++)
			{
				vkDestroyFramebuffer(m_pDevice->raw(), m_frameBuffers[i], nullptr);
				vkDestroyImageView(m_pDevice->raw(), m_imageViews[i], nullptr);
			}
			vkDestroySwapchainKHR(m_pDevice->raw(), m_swapchain, nullptr);

			m_swapchain = VK_NULL_HANDLE;
			m_imageCount = 0;
			delete[] m_images;
			m_images = nullptr;
			delete[] m_imageViews;
			m_imageViews = nullptr;
			delete[] m_frameBuffers;
			m_frameBuffers = nullptr;
			m_extent = {};
			m_pDevice = nullptr;
		}

		VkSwapchainKHR Swapchain::raw() const
		{
			if (m_swapchain == VK_NULL_HANDLE)throw NotCreatedException();
			return m_swapchain;
		}

		VkSwapchainKHR* Swapchain::ptr()
		{
			if (m_swapchain == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_swapchain;
		}

		VkFramebuffer Swapchain::getFramebuffer(u32 index) const
		{
			if (m_swapchain == VK_NULL_HANDLE)throw NotCreatedException();
			if (index >= m_imageCount)throw IllegalIndexException();
			return m_frameBuffers[index];
		}


		void Swapchain::chooseExtent(u32 width, u32 height)
		{
			const VkSurfaceCapabilitiesKHR& cap = m_pDevice->getPhysical()->surfaceCapabilities();
			if (cap.currentExtent.width != std::numeric_limits<uint32_t>::max())
			{
				m_extent = cap.currentExtent;
			}
			else
			{
				VkExtent2D actualExtent = { width, height };
				actualExtent.width = max(cap.minImageExtent.width, min(cap.maxImageExtent.width, actualExtent.width));
				actualExtent.height = max(cap.minImageExtent.height, min(cap.maxImageExtent.height, actualExtent.height));

				m_extent = actualExtent;
			}
		}

		void Swapchain::chooseImageCount()
		{
			m_imageCount = m_pDevice->getPhysical()->surfaceCapabilities().minImageCount + 1;
			if (m_pDevice->getPhysical()->surfaceCapabilities().maxImageCount > 0 && m_imageCount > m_pDevice->getPhysical()->surfaceCapabilities().maxImageCount)
			{
				m_imageCount = m_pDevice->getPhysical()->surfaceCapabilities().maxImageCount;
			}
		}

		void Swapchain::setupImages()
		{
			vkGetSwapchainImagesKHR(m_pDevice->raw(), m_swapchain, &m_imageCount, nullptr);
			m_images = new VkImage[m_imageCount];
			vkGetSwapchainImagesKHR(m_pDevice->raw(), m_swapchain, &m_imageCount, m_images);

			m_imageViews = new VkImageView[m_imageCount];
			for (u32 i = 0; i < m_imageCount; i++)
			{
				VkImageViewCreateInfo ivci;
				ivci.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
				ivci.pNext = nullptr;
				ivci.flags = 0;
				ivci.image = m_images[i];
				ivci.viewType = VK_IMAGE_VIEW_TYPE_2D;
				ivci.format = m_pDevice->getSurfaceFormat().format;
				ivci.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivci.components.g = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivci.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivci.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
				ivci.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
				ivci.subresourceRange.baseMipLevel = 0;
				ivci.subresourceRange.levelCount = 1;
				ivci.subresourceRange.baseArrayLayer = 0;
				ivci.subresourceRange.layerCount = 1;

				if (vkCreateImageView(m_pDevice->raw(), &ivci, nullptr, &m_imageViews[i]) != VK_SUCCESS)
				{
					throw VulkanException("failed to create swapchain image");
				}
			}
		}

		void Swapchain::setupFramebuffers(DepthImage* pDepthImage, RenderPass* pRenderPass)
		{
			m_frameBuffers = new VkFramebuffer[m_imageCount];

			for (u32 i = 0; i < m_imageCount; i++)
			{
				lw::DynamicArray<VkImageView> attachments = { m_imageViews[i], pDepthImage->view() };

				VkFramebufferCreateInfo fci;
				fci.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
				fci.pNext = nullptr;
				fci.flags = 0;
				fci.renderPass = pRenderPass->raw();
				fci.attachmentCount = attachments.size();
				fci.pAttachments = attachments.raw();
				fci.width = m_extent.width;
				fci.height = m_extent.height;
				fci.layers = 1;

				if (vkCreateFramebuffer(m_pDevice->raw(), &fci, nullptr, &m_frameBuffers[i]) != VK_SUCCESS)
				{
					throw VulkanException("failed to create framebuffer");
				}
			}
		}

	}
}