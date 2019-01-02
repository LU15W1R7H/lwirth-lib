#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "../../general/Exceptions.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class Semaphore;
		class Fence;
		class Queue;
		class Buffer;
		class Pipeline;

		class CommandPool
		{
		private:
			VkCommandPool	m_pool		= VK_NULL_HANDLE;
			Device*			m_pDevice	= nullptr;

		public:
			CommandPool() = default;
			~CommandPool();

			void create(Device* pDevice);
			void destroy();

			VkCommandPool raw() const;
			VkCommandPool* ptr();
		};

		class CommandBuffer
		{
		private:
			VkCommandBuffer		m_buffer	= VK_NULL_HANDLE;
			bool				m_recording	= false;

			const CommandPool*	m_pPool		= nullptr;
			const Device*		m_pDevice	= nullptr;

		public:
			CommandBuffer() = default;
			~CommandBuffer();

			void allocate(const Device* pDevice, const CommandPool* pPool);
			void free();

			bool exists() const;

			VkCommandBuffer raw() const;
			VkCommandBuffer* ptr();

			void begin();
			void end();
			void submit(const Queue* pQueue, VkPipelineStageFlags stage, const Semaphore* waitSemaphore, const Semaphore* signaleSemaphore, const Fence* fence = nullptr);

			void beginSingleTime();
			void endAndExecuteAndFree(const Queue* pQueue);


			void cmdBindPipeline(VkPipelineBindPoint bindPoint, const Pipeline& pipeline) const;
			void cmdSetViewport(const VkViewport& viewport) const;
			void cmdSetViewport(const VkViewport* pViewports, u32 count, u32 firstViewport) const;
			void cmdSetScissor(const VkRect2D& scissor) const;
			void cmdSetScissor(const VkRect2D* pScissors, u32 count, u32 firstScissor) const;
			void cmdBindVertexBuffer(const Buffer& buffer) const;
			void cmdBindVertexBuffer(const Buffer* pBuffer, u32 firstBinding, u32 bindingCount, const VkDeviceSize* pOffsets = nullptr) const;
			void cmdBindIndexBuffer(const Buffer& buffer, VkIndexType indexType, VkDeviceSize offset = 0) const;
			void cmdDraw(u32 vertexCount, u32 firstVertex = 0, u32 instanceCount = 1, u32 firstInstance = 0) const;
			void cmdDrawIndexed(u32 indexCount, u32 firstIndex = 0, u32 vertexOffset = 0, u32 instanceCount = 0, u32 firstInstance = 0) const;
			void cmdCopyBuffer(const Buffer& src, const Buffer& dst, const VkBufferCopy* pRegions, u32 regionCount) const;
			
			//void cmdBeginRenderPass(const VkRenderPassBeginInfo& rpbi, VkSubpassContents sc);
			//void cmdBindDescriptorSets(const DescriptorSet*)
			//void cmdBindIndexBuffer(const GPUBuffer* pBuffer);
			//void cmdBindPipeline(const )
		};

	}
}

