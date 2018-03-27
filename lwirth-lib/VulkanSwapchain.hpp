#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "DynamicArray.hpp"

namespace lw
{
	class Frame;
	namespace VK
	{
		class DepthImage;
		class Surface;
		class Device;
		class RenderPass;


		class API Swapchain
		{
		private:
			VkSwapchainKHR	m_swapchain		= VK_NULL_HANDLE;
			U32				m_imageCount	= 0;
			VkImage*		m_images		= nullptr;
			VkImageView*	m_imageViews	= nullptr;
			VkFramebuffer*	m_frameBuffers	= nullptr;
			VkExtent2D		m_extent		= {};

			Device*			m_pDevice		= nullptr;

		public:
			Swapchain() = default;
			~Swapchain() = default;

			void create(Device* pDevice, Surface* pSurface, DepthImage* pDepthImage, RenderPass* pRenderPass, U32 width, U32 height, Swapchain* pOldSwapchain = nullptr);
			void destroy();

			VkSwapchainKHR raw() const;
			VkSwapchainKHR* ptr();
			
			VkFramebuffer getFramebuffer(U32 index) const;
		private:
			void chooseExtent(U32 width, U32 height);
			void chooseImageCount();

			void setupImages();
			void setupFramebuffers(DepthImage* pDepthImage, RenderPass* pRenderPass);
		};

	}
}