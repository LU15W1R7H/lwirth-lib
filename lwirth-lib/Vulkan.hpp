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
#include "VulkanImage.hpp"
#include "VulkanDepthImage.hpp"
#include "VulkanPipeline.hpp"
#include "VulkanCommands.hpp"
#include "VulkanDescriptors.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"

#include "SimpleBrush2D.hpp"
#include "SimpleBrush3D.hpp"



namespace lw
{
	namespace VK
	{
		class Vulkan
		{
			friend class SimpleBrush2D;
			friend class SimpleBrush3D;
			friend class Visualizer;

		private:
			const Frame* m_pFrame = nullptr;

			Instance m_instance									= {};
			Surface m_surface									= {};
			PhysicalDeviceContainer m_physicalDeviceContatiner	= {};
			Device m_mainDevice									= {};
			RenderPass m_renderPass								= {};
			DepthImage m_depthImage								= {};
			Swapchain m_swapchain								= {};
			
			CommandPool m_commandPool							= {};
			Semaphore m_semaphoreImageAvailable					= {};
			Semaphore m_semaphoreRenderingDone					= {};
			CommandBuffer m_drawingCommandBuffer				= {};
			DescriptorPool m_descriptorPool						= {};
			Fence m_fence										= {};

			SimpleBrush2D* m_pSimpleBrush2D = nullptr;
			SimpleBrush3D* m_pSimpleBrush3D = nullptr;

			u32 m_screenWidth = 0;
			u32 m_screenHeight = 0;

			u32 m_imageIndex = 0;

			lw::List<Buffer> m_bufferDestroyQueue = {};
		public:
			Vulkan() = default;
			~Vulkan() = default;

			//void init();
			void start(const Frame* pFrame);
			void destroy();

			SimpleBrush2D* getSimpleBrush2D();
			SimpleBrush3D* getSimpleBrush3D();
			void preDraw();
			void postDraw();

			void resize(u32 width, u32 height);

			void submitBufferToDestroy(const Buffer& buffer);
		private:
			void recreateSwapchain();
			
			void destroyPendingBuffers();
		};
	}
	

}