#pragma once

#include "../Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Frame.hpp"

#include "vulkan/VulkanHelper.hpp"
#include "vulkan/VulkanInstance.hpp"
#include "vulkan/VulkanSurface.hpp"
#include "vulkan/VulkanPhysicalDevices.hpp"
#include "vulkan/VulkanDevice.hpp"
#include "vulkan/VulkanSwapchain.hpp"
#include "vulkan/VulkanRenderPass.hpp"
#include "vulkan/VulkanShader.hpp"
#include "vulkan/VulkanImage.hpp"
#include "vulkan/VulkanDepthImage.hpp"
#include "vulkan/VulkanPipeline.hpp"
#include "vulkan/VulkanCommands.hpp"
#include "vulkan/VulkanDescriptors.hpp"
#include "vulkan/VulkanBuffer.hpp"
#include "vulkan/VulkanSemaphore.hpp"
#include "vulkan/VulkanFence.hpp"

#include "SimpleBrush2D.hpp"
#include "SimpleBrush3D.hpp"



namespace lw
{
	class Renderer
	{
		friend class SimpleBrush2D;
		friend class SimpleBrush3D;
		friend class Visualizer;

	private:
		const Frame* m_pFrame = nullptr;

		VK::Instance m_instance									= {};
		VK::Surface m_surface									= {};
		VK::PhysicalDeviceContainer m_physicalDeviceContatiner	= {};
		VK::Device m_mainDevice									= {};
		VK::RenderPass m_renderPass								= {};
		VK::DepthImage m_depthImage								= {};
		VK::Swapchain m_swapchain								= {};
		
		VK::CommandPool m_commandPool							= {};
		VK::Semaphore m_semaphoreImageAvailable					= {};
		VK::Semaphore m_semaphoreRenderingDone					= {};
		VK::CommandBuffer m_drawingCommandBuffer				= {};
		VK::DescriptorPool m_descriptorPool						= {};
		VK::Fence m_fence										= {};

		SimpleBrush2D* m_pSimpleBrush2D = nullptr;
		


		SimpleBrush3D* m_pSimpleBrush3D = nullptr;

		u32 m_screenWidth = 0;
		u32 m_screenHeight = 0;

		u32 m_imageIndex = 0;

		lw::List<VK::Buffer> m_bufferDestroyQueue = {};
	public:
		Renderer() = default;
		~Renderer() = default;

		//void init();
		void start(const Frame* pFrame);
		void destroy();

		SimpleBrush2D* getSimpleBrush2D();
		SimpleBrush3D* getSimpleBrush3D();
		void preDraw();
		void postDraw();

		void resize(u32 width, u32 height);

		void submitBufferToDestroy(const VK::Buffer& buffer);
	private:
		void recreateSwapchain();
		
		void destroyPendingBuffers();
	};
	
}