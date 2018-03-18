#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "Exceptions.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class Semaphore;
		class Fence;
		class Queue;

		class API CommandPool
		{
		private:
			VkCommandPool	m_pool		= VK_NULL_HANDLE;
			Device*			m_pDevice	= nullptr;

		public:
			CommandPool();
			~CommandPool();

			void create(Device* pDevice);
			void destroy();

			VkCommandPool raw() const;
			VkCommandPool* ptr();
		};

		class API CommandBuffer
		{
		private:
			VkCommandBuffer		m_buffer	= VK_NULL_HANDLE;
			bool				m_recording	= false;

			const CommandPool*	m_pPool		= nullptr;
			const Device*		m_pDevice	= nullptr;

		public:
			CommandBuffer();
			~CommandBuffer();

			void allocate(const Device* pDevice, const CommandPool* pPool);
			void free();

			VkCommandBuffer raw() const;
			VkCommandBuffer* ptr();

			void begin();
			void end();
			void submit(const Queue* pQueue, VkPipelineStageFlags stage, const Semaphore* waitSemaphore, const Semaphore* signaleSemaphore, const Fence* fence = nullptr);

			void beginSingleTime();
			void endAndExecuteAndFree(const Queue* pQueue);

			//void cmdBeginRenderPass(const VkRenderPassBeginInfo& rpbi, VkSubpassContents sc);
			//void cmdBindDescriptorSets(const DescriptorSet*)
			//void cmdBindIndexBuffer(const GPUBuffer* pBuffer);
			//void cmdBindPipeline(const )
		};

	}
}

