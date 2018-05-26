#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "List.hpp"

namespace lw
{
	class Frame;
	namespace VK
	{
		class DepthImage;
		class Surface;
		class Device;
		class RenderPass;


		class Swapchain
		{
		private:
			VkSwapchainKHR	m_swapchain		= VK_NULL_HANDLE;
			u32				m_imageCount	= 0;
			VkImage*		m_images		= nullptr;
			VkImageView*	m_imageViews	= nullptr;
			VkFramebuffer*	m_frameBuffers	= nullptr;
			VkExtent2D		m_extent		= {};

			Device*			m_pDevice		= nullptr;

		public:
			Swapchain() = default;
			~Swapchain() = default;

			void create(Device* pDevice, Surface* pSurface, DepthImage* pDepthImage, RenderPass* pRenderPass, u32 width, u32 height, Swapchain* pOldSwapchain = nullptr);
			void destroy();

			VkSwapchainKHR raw() const;
			VkSwapchainKHR* ptr();
			
			VkFramebuffer getFramebuffer(u32 index) const;
		private:
			void chooseExtent(u32 width, u32 height);
			void chooseImageCount();

			void setupImages();
			void setupFramebuffers(DepthImage* pDepthImage, RenderPass* pRenderPass);
		};

	}
}