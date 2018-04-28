#include "stdafx.hpp"
#include "VulkanBuffer.hpp"

#include "Exceptions.hpp"
#include "VulkanHelper.hpp"
#include "VulkanDevice.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanCommands.hpp"
#include "VulkanQueues.hpp"

namespace lw
{
	namespace VK
	{

		VkBuffer Buffer::raw() const
		{
			if (!exists())throw NotAllocatedException();
			return m_buffer;
		}

		VkDeviceMemory Buffer::memory() const
		{
			if (!exists())throw NotAllocatedException();
			return m_memory;
		}

		VkDeviceSize Buffer::size() const
		{
			return m_size;
		}

		VkBuffer* Buffer::ptr()
		{
			if (!exists())throw NotAllocatedException();
			return &m_buffer;
		}

		bool Buffer::exists() const
		{
			return m_buffer != VK_NULL_HANDLE;
		}

		void Buffer::s_allocate(const Device* pDevice, VkBuffer* pBuffer, VkDeviceMemory* pMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkSharingMode mode)
		{
			const SelectedQueueFamilies* queueFamilies = pDevice->getPhysical()->queueFamiles();

			VkBufferCreateInfo bci;
			bci.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
			bci.pNext = nullptr;
			bci.flags = 0;
			bci.size = size;
			bci.usage = usage;
			bci.sharingMode = mode;
			if (mode == VK_SHARING_MODE_EXCLUSIVE)
			{
				bci.queueFamilyIndexCount = 0;
				bci.pQueueFamilyIndices = nullptr;
			}
			else
			{
				if (!queueFamilies->complete())
				{
					throw VulkanException("Queue family indices are not complete");
				}
				lw::List<u32> familyIndices = queueFamilies->getIndices();
				bci.queueFamilyIndexCount = familyIndices.size();
				bci.pQueueFamilyIndices = familyIndices.raw();
			}

			if (vkCreateBuffer(pDevice->raw(), &bci, nullptr, pBuffer) != VK_SUCCESS)
			{
				throw VulkanException("failed to create buffer");
			}

			VkMemoryRequirements mr;
			vkGetBufferMemoryRequirements(pDevice->raw(), *pBuffer, &mr);

			VkMemoryAllocateInfo mai;
			mai.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
			mai.pNext = nullptr;
			mai.allocationSize = mr.size;
			mai.memoryTypeIndex = findMemoryTypeIndex(pDevice->getPhysical(), mr.memoryTypeBits, memoryProperties);

			if (vkAllocateMemory(pDevice->raw(), &mai, nullptr, pMemory) != VK_SUCCESS)
			{
				throw VulkanException("failed to allocate buffer memory");
			}

			vkBindBufferMemory(pDevice->raw(), *pBuffer, *pMemory, 0);
		}

		void Buffer::s_copy(const Device* device, const CommandPool* commandPool, const Queue* pQueue, VkBuffer src, VkBuffer dst, VkDeviceSize size)
		{
			CommandBuffer commandBuffer;
			commandBuffer.allocate(device, commandPool);
			commandBuffer.beginSingleTime();

			VkBufferCopy bufferCopy;
			bufferCopy.srcOffset = 0;
			bufferCopy.dstOffset = 0;
			bufferCopy.size = size;
			vkCmdCopyBuffer(commandBuffer.raw(), src, dst, 1, &bufferCopy);

			commandBuffer.endAndExecuteAndFree(pQueue);
		}

		void Buffer::destroy()
		{
			if (!exists())return;

			vkDestroyBuffer(m_pDevice->raw(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;

			m_size = 0;
			m_pDevice = nullptr;
		}

		void CPUBuffer::allocate(const Device* pDevice, u32 byteSize, VkBufferUsageFlags usage, VkSharingMode sharingMode)
		{
			if (exists())throw AlreadyCreatedException();

			if (byteSize == 0)
			{
				throw VulkanException("invalid buffer size");
			}

			m_pDevice = pDevice;
			m_size = byteSize;

			s_allocate(m_pDevice, &m_buffer, &m_memory, m_size, usage, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, sharingMode);
		}

		void CPUBuffer::destroy()
		{
			if (!exists())return;
			if (m_mapped)throw VulkanException("buffer can't be destroyed when mapped");

			vkDestroyBuffer(m_pDevice->raw(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;

			m_size = 0;
			m_pDevice = nullptr;
		}

		void* CPUBuffer::map()
		{
			if (!exists())throw NotAllocatedException();
			if (m_mapped)throw VulkanException("buffer is already mapped");

			void* data;
			vkMapMemory(m_pDevice->raw(), m_memory, 0, m_size, 0, &data);
			m_mapped = true;

			return data;
		}

		void CPUBuffer::unmap()
		{
			if (!exists())throw NotAllocatedException();
			if (!m_mapped)throw VulkanException("buffer is not mapped");

			vkUnmapMemory(m_pDevice->raw(), m_memory);
			m_mapped = false;
		}


		void StagingBuffer::allocate(const Device* pDevice, u32 byteSize)
		{
			if (exists())throw AlreadyCreatedException();

			if (byteSize == 0)
			{
				throw VulkanException("invalid buffer size");
			}

			m_pDevice = pDevice;
			m_size = byteSize;

			s_allocate(
				m_pDevice, &m_buffer, &m_memory, m_size,
				VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
				VK_SHARING_MODE_EXCLUSIVE
			);
		}

		void StagingBuffer::destroy()
		{
			if (!exists())return;
			if (m_mapped)throw VulkanException("buffer can't be destroyed when mapped");

			vkDestroyBuffer(m_pDevice->raw(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;

			m_size = 0;
			m_pDevice = nullptr;
		}

		void* StagingBuffer::map()
		{
			if (!exists())throw NotAllocatedException();
			if (m_mapped)throw VulkanException("buffer is already mapped");

			void* data;
			vkMapMemory(m_pDevice->raw(), m_memory, 0, m_size, 0, &data);
			m_mapped = true;

			return data;
		}

		void StagingBuffer::unmap()
		{
			if (!exists())throw NotAllocatedException();
			if (!m_mapped)throw VulkanException("buffer is not mapped");

			vkUnmapMemory(m_pDevice->raw(), m_memory);
			m_mapped = false;
		}

		void GPUBuffer::allocate(const Device* pDevice, const CommandPool* commandPool, const Queue* pQueue, const StagingBuffer* pStagingBuffer, VkBufferUsageFlags usage, VkSharingMode sharingMode)
		{
			if (exists())throw AlreadyCreatedException();

			m_pDevice = pDevice;
			m_size = pStagingBuffer->size();

			s_allocate(
				m_pDevice, &m_buffer, &m_memory, m_size,
				usage | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
				VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, sharingMode
			);

			s_copy(m_pDevice, commandPool, pQueue, pStagingBuffer->raw(), m_buffer, m_size);
		}

		void GPUBuffer::destroy()
		{
			if (!exists())return;

			vkDestroyBuffer(m_pDevice->raw(), m_buffer, nullptr);
			m_buffer = VK_NULL_HANDLE;
			vkFreeMemory(m_pDevice->raw(), m_memory, nullptr);
			m_memory = VK_NULL_HANDLE;

			m_size = 0;
			m_pDevice = nullptr;
		}

	}
}
