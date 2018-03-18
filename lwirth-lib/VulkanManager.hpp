#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Frame.hpp"

#include "VulkanHelper.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanDevice.hpp"
#include "VulkanSwapchain.hpp"
#include "VulkanRenderPass.hpp"
#include "VulkanShader.hpp"
#include "VulkanDepthImage.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanCommands.hpp"
#include "VulkanDescriptors.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"

#include "SimpleBrush2D.hpp"



namespace lw
{
	namespace VK
	{
		class API VulkanManager
		{
		private:
			const Frame* m_pFrame = nullptr;

			Instance m_instance;
			Surface m_surface;
			PhysicalDeviceContainer m_physicalDeviceContatiner;
			Device m_device;
			RenderPass m_renderPass;
			DepthImage m_depthImage;
			Swapchain m_swapchain;

			CommandPool m_commandPool;
			Semaphore m_semaphoreImageAvailable;
			Semaphore m_semaphoreRenderingDone;
			CommandBuffer m_drawingCommandBuffer;
			DescriptorPool m_descriptorPool;
			Fence m_fence;

			SimpleBrush2D m_simpleBrush2D;

			U32 m_screenWidth;
			U32 m_screenHeight;

			U32 m_imageIndex;
		public:
			VulkanManager() {}
			~VulkanManager() {}

			//void init();
			void start(const Frame* pFrame);
			void destroy();

			SimpleBrush2D* preDraw();
			void postDraw();

			void resize(U32 width, U32 height);
		private:
			void recreateSwapchain();
		};
	}
	

}