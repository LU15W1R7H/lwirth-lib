#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>


namespace lw
{
	namespace VK
	{
		class Device;
		class CommandPool;
		class Queue;
		
		class API Buffer //virtual
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

			bool exists() const;

		protected:
			static void s_allocate(const Device* pDevice, VkBuffer* pBuffer, VkDeviceMemory* pMemory, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags memoryProperties, VkSharingMode mode);
			static void s_copy(const Device* device, const CommandPool* commandPool, const Queue* pQueue, VkBuffer src, VkBuffer dst, VkDeviceSize size);
		};

		class API CPUBuffer : public Buffer
		{
		private:
			bool m_mapped = false;
		public:
			void allocate(const Device* pDevice, U32 byteSize, VkBufferUsageFlags usage, VkSharingMode sharingMode);
			void destroy() override;

			void* map();
			void unmap();
		};

		class API StagingBuffer : public Buffer
		{
		private:
			bool m_mapped = false;
		public:
			void allocate(const Device* pDevice, U32 byteSize);
			void destroy() override;

			void* map();
			void unmap();
		};

		class API GPUBuffer : public Buffer
		{
		private:

		public:
			void allocate(const Device* pDevice, const CommandPool* commandPool, const Queue* queue, const StagingBuffer* pStagingBuffer, VkBufferUsageFlags usage, VkSharingMode sharingMode);
			void destroy() override;
		};
		

	}
}