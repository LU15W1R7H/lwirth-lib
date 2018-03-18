#include "stdafx.hpp"
#include "VulkanCommands.hpp"

#include "Exceptions.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanQueues.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"

namespace lw
{
	namespace VK
	{
		CommandPool::CommandPool()
		{
		}

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
			poolInfo.queueFamilyIndex = static_cast<U32>(m_pDevice->getPhysical()->queueFamiles()->getGraphics()->getIndex());

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

		
		
		CommandBuffer::CommandBuffer()
		{
		}

		CommandBuffer::~CommandBuffer()
		{
			if (m_buffer != VK_NULL_HANDLE) throw NotFreedException();
		}

		void CommandBuffer::allocate(const Device* pDevice, const CommandPool* pPool)
		{
			if (m_buffer != VK_NULL_HANDLE)throw AlreadyAllocatedException();

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
			if (m_buffer == VK_NULL_HANDLE)return;
			if (m_recording) throw IllegalStateException();

			vkFreeCommandBuffers(m_pDevice->raw(), m_pPool->raw(), 1, &m_buffer);
			m_buffer = VK_NULL_HANDLE;
			m_pPool = nullptr;
			m_pDevice = nullptr;
		}

		VkCommandBuffer CommandBuffer::raw() const
		{
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
			return m_buffer;
		}

		VkCommandBuffer* CommandBuffer::ptr()
		{
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
			return &m_buffer;
		}

		void CommandBuffer::begin()
		{
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
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
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
			if (!m_recording) throw IllegalStateException();

			if (vkEndCommandBuffer(m_buffer) != VK_SUCCESS)
			{
				throw VulkanException("failed to end command buffer");
			}
			m_recording = false;
		}

		void CommandBuffer::submit(const Queue* pQueue, VkPipelineStageFlags stage, const Semaphore* waitSemaphore, const Semaphore* signaleSemaphore, const Fence* fence)
		{
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
			if (m_recording) throw IllegalStateException();

			pQueue->submit(this, waitSemaphore, signaleSemaphore, stage, fence);
		}

		void CommandBuffer::beginSingleTime()
		{
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
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
			if (m_buffer == VK_NULL_HANDLE) throw NotAllocatedException();
			if (!m_recording) throw IllegalStateException();

			CommandBuffer::end();

			VK::Fence fence;
			fence.create(m_pDevice);
			pQueue->submit(this, nullptr, nullptr, {}, &fence);
			fence.wait();
			fence.destroy();

			CommandBuffer::free();
		}

}
}