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
		class API Vulkan
		{
			friend class SimpleBrush2D;
			friend class SimpleBrush3D;
			friend class Visualizer;

		private:
			const Frame* m_pFrame = nullptr;

			Instance m_instance									= {};
			Surface m_surface									= {};
			PhysicalDeviceContainer m_physicalDeviceContatiner	= {};
			Device m_device										= {};
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

			U32 m_screenWidth = 0;
			U32 m_screenHeight = 0;

			U32 m_imageIndex = 0;

			lw::DynamicArray<Buffer> m_bufferDestroyQueue = {};
		public:
			Vulkan() = default;
			~Vulkan() = default;

			//void init();
			void start(const Frame* pFrame);
			void destroy();

			SimpleBrush2D* getSimpleBrush2D();
			void preDraw();
			void postDraw();

			void resize(U32 width, U32 height);

			void submitBufferToDestroy(const Buffer& buffer);
		private:
			void recreateSwapchain();
			
			void destroyPendingBuffers();
		};
	}
	

}