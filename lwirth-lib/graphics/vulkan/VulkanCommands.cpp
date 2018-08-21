#include "../../stdafx.hpp"
#include "VulkanCommands.hpp"

#include "../../general/Exceptions.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanQueues.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanPipeline.hpp"

namespace lw
{
	namespace VK
	{
		CommandPool::~CommandPool()
		{
			if (m_pool != VK_NULL_HANDLE) throw NotDestroyedException();
		}

		void CommandPool::create(Device* pDevice)
		{
			if (m_pool != VK_NULL_HANDLE) throw AlreadyCreatedException();

			m_pDevice = pDevice;

			VkCommandPoolCreateInfo poolInfo = {};
			poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
			poolInfo.pNext = nullptr;
			poolInfo.flags = 0;
			poolInfo.queueFamilyIndex = static_cast<u32>(m_pDevice->getPhysical()->queueFamiles()->getGraphics()->getIndex());

			if (vkCreateCommandPool(m_pDevice->raw(), &poolInfo, nullptr, &m_pool) != VK_SUCCESS)
			{
				throw VulkanException("failed to create command pool");
			}
		}

		void CommandPool::destroy()
		{
			if (m_pool == VK_NULL_HANDLE)return;

			vkDestroyCommandPool(m_pDevice->raw(), m_pool, nullptr);
			m_pool = VK_NULL_HANDLE;
			m_pDevice = nullptr;
		}

		VkCommandPool CommandPool::raw() const
		{
			if (m_pool == VK_NULL_HANDLE)throw NotCreatedException();
			return m_pool;
		}

		VkCommandPool* CommandPool::ptr()
		{
			if (m_pool == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_pool;
		}

		CommandBuffer::~CommandBuffer()
		{
			if (exists()) throw NotFreedException();
		}

		void CommandBuffer::allocate(const Device* pDevice, const CommandPool* pPool)
		{
			if (exists())throw AlreadyAllocatedException();

			m_pDevice = pDevice;
			m_pPool = pPool;

			VkCommandBufferAllocateInfo cbai;
			cbai.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
			cbai.pNext = nullptr;
			cbai.commandPool = m_pPool->raw();
			cbai.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
			cbai.commandBufferCount = 1;

			if (vkAllocateCommandBuffers(m_pDevice->raw(), &cbai, &m_buffer) != VK_SUCCESS)
			{
				throw VulkanException("failed to allocate command buffer");
			}
		}

		void CommandBuffer::free()
		{
			if (!exists())return;
			if (m_recording) throw IllegalStateException();

			vkFreeCommandBuffers(m_pDevice->raw(), m_pPool->raw(), 1, &m_buffer);
			m_buffer = VK_NULL_HANDLE;
			m_pPool = nullptr;
			m_pDevice = nullptr;
		}

		bool CommandBuffer::exists() const
		{
			return m_buffer != VK_NULL_HANDLE;
		}

		VkCommandBuffer CommandBuffer::raw() const
		{
			if (!exists()) throw NotAllocatedException();
			return m_buffer;
		}

		VkCommandBuffer* CommandBuffer::ptr()
		{
			if (!exists()) throw NotAllocatedException();
			return &m_buffer;
		}

		void CommandBuffer::begin()
		{
			if (!exists()) throw NotAllocatedException();
			if (m_recording) throw IllegalStateException();

			VkCommandBufferBeginInfo ccbi;
			ccbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			ccbi.pNext = nullptr;
			ccbi.flags = 0;
			ccbi.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(m_buffer, &ccbi) != VK_SUCCESS)
			{
				throw VulkanException("failed to begin command buffer");
			}
		}

		void CommandBuffer::end()
		{
			if (!exists()) throw NotAllocatedException();
			if (!m_recording) throw IllegalStateException();

			if (vkEndCommandBuffer(m_buffer) != VK_SUCCESS)
			{
				throw VulkanException("failed to end command buffer");
			}
			m_recording = false;
		}

		void CommandBuffer::submit(const Queue* pQueue, VkPipelineStageFlags stage, const Semaphore* waitSemaphore, const Semaphore* signaleSemaphore, const Fence* fence)
		{
			if (!exists()) throw NotAllocatedException();
			if (m_recording) throw IllegalStateException();

			pQueue->submit(this, waitSemaphore, signaleSemaphore, stage, fence);
		}

		void CommandBuffer::beginSingleTime()
		{
			if (!exists()) throw NotAllocatedException();
			if (m_recording) throw IllegalStateException();

			VkCommandBufferBeginInfo cbbi;
			cbbi.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
			cbbi.pNext = nullptr;
			cbbi.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
			cbbi.pInheritanceInfo = nullptr;

			if (vkBeginCommandBuffer(m_buffer, &cbbi) != VK_SUCCESS)
			{
				throw VulkanException("failed to begin single time command buffer");
			}
			m_recording = true;
		}

		void CommandBuffer::endAndExecuteAndFree(const Queue* pQueue)
		{
			if (!exists()) throw NotAllocatedException();
			if (!m_recording) throw IllegalStateException();

			CommandBuffer::end();

			VK::Fence fence;
			fence.create(m_pDevice);
			pQueue->submit(this, nullptr, nullptr, {}, &fence);
			fence.wait();
			fence.destroy();

			CommandBuffer::free();
		}

		void CommandBuffer::cmdBindPipeline(VkPipelineBindPoint bindPoint, const Pipeline& pipeline) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdBindPipeline(m_buffer, bindPoint, pipeline.raw());
		}

		void CommandBuffer::cmdSetViewport(const VkViewport & viewport) const
		{
			cmdSetViewport(&viewport, 1, 0);
		}

		void CommandBuffer::cmdSetViewport(const VkViewport * pViewports, u32 count, u32 firstViewport) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdSetViewport(m_buffer, firstViewport, count, pViewports);
		}

		void CommandBuffer::cmdSetScissor(const VkRect2D & scissor) const
		{
			cmdSetScissor(&scissor, 1, 0);
		}

		void CommandBuffer::cmdSetScissor(const VkRect2D * pScissors, u32 count, u32 firstScissor) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdSetScissor(m_buffer, firstScissor, count, pScissors);
		}

		void CommandBuffer::cmdBindVertexBuffer(const Buffer & buffer) const
		{
			cmdBindVertexBuffer(&buffer, 0, 1);
		}

		void CommandBuffer::cmdBindVertexBuffer(const Buffer* pBuffer, u32 firstBinding, u32 bindingCount, const VkDeviceSize * pOffsets) const
		{
			if (!exists()) throw NotAllocatedException();
			static VkDeviceSize nullOffset[] = { 0 };
			vkCmdBindVertexBuffers(m_buffer, firstBinding, bindingCount, pBuffer->ptr(), pOffsets ? pOffsets : nullOffset);
		}

		void CommandBuffer::cmdDraw(u32 vertexCount, u32 firstVertex, u32 instanceCount, u32 firstInstance) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdDraw(m_buffer, vertexCount, instanceCount, firstVertex, firstInstance);
		}

		void CommandBuffer::cmdDrawIndexed(u32 indexCount, u32 firstIndex, u32 vertexOffset, u32 instanceCount, u32 firstInstance) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdDrawIndexed(m_buffer, indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
		}

		void CommandBuffer::cmdCopyBuffer(const Buffer& src, const Buffer& dst, const VkBufferCopy * pRegions, u32 regionCount) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdCopyBuffer(m_buffer, src.raw(), dst.raw(), regionCount, pRegions);
		}

		void CommandBuffer::cmdBindIndexBuffer(const Buffer & buffer, VkIndexType indexType, VkDeviceSize offset) const
		{
			if (!exists()) throw NotAllocatedException();
			vkCmdBindIndexBuffer(m_buffer, buffer.raw(), offset, indexType);
		}

}
}