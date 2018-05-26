#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <string>


namespace lw
{
	namespace VK
	{
		class Device;
		class CommandPool;
		class Queue;
		
		class Buffer //virtual
		{
		protected:
			VkBuffer			m_buffer	= VK_NULL_HANDLE;
			VkDeviceMemory		m_memory	= VK_NULL_HANDLE;
			VkDeviceSize		m_size		= 0;

			const Device* m_pDevice = nullptr;
		public:
			Buffer() = default;
			~Buffer() = default;

			virtual void destroy();

			VkBuffer raw() const;
			VkDeviceMemory memory() const;
			VkDeviceSize size() const;
			VkBuffer* ptr();
			const VkBuffer* ptr() const;

			bool exists() const;

		protected:
			static void s_allocate(const Device* pDevice, VkBuffer* pBuffer, VkDeviceMemory* pMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkSharingMode mode);
			static void s_copy(const Device* device, const CommandPool* commandPool, const Queue* pQueue, VkBuffer src, VkBuffer dst, VkDeviceSize size);
		};

		class CPUBuffer : public Buffer
		{
		private:
			bool m_mapped = false;
		public:
			void allocate(const Device* pDevice, u32 byteSize, VkBufferUsageFlags usage, VkSharingMode sharingMode);
			void destroy() override;

			void* map();
			void unmap();
		};

		class StagingBuffer : public Buffer
		{
		private:
			bool m_mapped = false;
		public:
			void allocate(const Device* pDevice, u32 byteSize);
			void destroy() override;

			void* map();
			void unmap();

			void allocateAndReadImage(const Device* pDevice, const std::string& path);
		};

		class GPUBuffer : public Buffer
		{
		private:

		public:
			void allocate(const Device* pDevice, const CommandPool* commandPool, const Queue* queue, const StagingBuffer* pStagingBuffer, VkBufferUsageFlags usage, VkSharingMode sharingMode);
			void destroy() override;
		};
		

	}
}