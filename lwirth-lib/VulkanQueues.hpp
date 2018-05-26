#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>


namespace lw
{
	namespace VK
	{
		class Device;
		class PhysicalDevice;
		class CommandBuffer;
		class Semaphore;
		class Fence;

		class QueueFamilies;
		class QueueFamily;
		class Queue;

		class LWAPI QueueFamilies
		{
		private:
			QueueFamily* m_families = nullptr;
			u32 m_count = 0;

		public:
			bool exists() const;
			void init(const PhysicalDevice* pDevice);
			void deinit();

			u32 count() const;
			const QueueFamily* get(u32 index) const;
		};

		class LWAPI QueueFamily //pure virtual
		{
		private:
			int m_index = -1;
			VkQueueFamilyProperties m_properties = {};

		public:
			bool exists() const;
			void init(u32 index, const VkQueueFamilyProperties& pProperties);
			void deinit();

			u32 getIndex() const;
			const VkQueueFamilyProperties& getProperties() const;
			Queue getQueue(const Device* pDevice, u32 index) const;
		};

		class LWAPI Queue
		{
		private:
			VkQueue m_queue;

		public:
			void init(const Device* pDevice, const QueueFamily* pFamily, u32 index);
			void clear();

			bool exists() const;
			VkQueue raw() const;
			VkQueue* ptr();

			void wait() const;
			void submit(const CommandBuffer* pCmdBuffer, const Semaphore* pWaitSemaphore, const Semaphore* pSignalSemaphore, VkPipelineStageFlags pipelineStage, const Fence* pFence = nullptr) const;
		};
	}
}