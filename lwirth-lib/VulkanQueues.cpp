#include "stdafx.hpp"
#include "VulkanQueues.hpp"

#include "VulkanPhysicalDevices.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"
#include "VulkanSemaphore.hpp"
#include "VulkanFence.hpp"
#include "Exceptions.hpp"

namespace lw
{
	namespace VK
	{

		void Queue::init(const Device* pDevice, const QueueFamily* pFamily, u32 index)
		{
			if (exists())throw AlreadyInitializedException();
			vkGetDeviceQueue(pDevice->raw(), pFamily->getIndex(), index, &m_queue);
		}

		void Queue::clear()
		{
			if (!exists()) return;
			m_queue = VK_NULL_HANDLE;
		}

		bool Queue::exists() const
		{
			return m_queue != VK_NULL_HANDLE;
		}

		VkQueue Queue::raw() const
		{
			if (!exists())throw NotInitializedException();
			return m_queue;
		}

		VkQueue* Queue::ptr()
		{
			if (!exists())throw NotInitializedException();
			return &m_queue;
		}

		void Queue::wait() const
		{
			if (!exists())throw NotInitializedException();
			vkQueueWaitIdle(m_queue);
		}

		void Queue::submit(const CommandBuffer* pCmdBuffer, const Semaphore* pWaitSemaphore, const Semaphore* pSignalSemaphore, VkPipelineStageFlags pipelineStage, const Fence* pFence) const
		{
			if (!exists())throw NotInitializedException();

			VkSubmitInfo si;
			si.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
			si.pNext = nullptr;
			VkSemaphore waitSemaphore;
			if (pWaitSemaphore != nullptr)
			{
				waitSemaphore = pWaitSemaphore->raw();
				si.waitSemaphoreCount = 1;
				si.pWaitSemaphores = &waitSemaphore;
			}
			else
			{
				si.waitSemaphoreCount = 0;
				si.pWaitSemaphores = nullptr;
			}
			si.pWaitDstStageMask = &pipelineStage;
			si.commandBufferCount = 1;
			VkCommandBuffer buffer = pCmdBuffer->raw();
			si.pCommandBuffers = &buffer;
			VkSemaphore signalSemaphore;
			if (pSignalSemaphore != nullptr)
			{
				signalSemaphore = pSignalSemaphore->raw();
				si.signalSemaphoreCount = 1;
				si.pSignalSemaphores = &signalSemaphore;
			}
			else
			{
				si.signalSemaphoreCount = 0;
				si.pSignalSemaphores = nullptr;
			}
			
			vkQueueSubmit(m_queue, 1, &si, pFence != nullptr ? pFence->raw() : VK_NULL_HANDLE);
		}


		bool QueueFamily::exists() const
		{
			return m_index >= 0;
		}

		void QueueFamily::init(u32 index, const VkQueueFamilyProperties& pProperties)
		{
			m_index = index;
			m_properties = pProperties;
		}

		void QueueFamily::deinit()
		{
			if (!exists())return;
			m_index = -1;
			m_properties = {};
		}

		u32 QueueFamily::getIndex() const
		{
			if (!exists())throw NotInitializedException();
			return m_index;
		}

		const VkQueueFamilyProperties & QueueFamily::getProperties() const
		{
			if (!exists())throw NotInitializedException();
			return m_properties;
		}

		Queue QueueFamily::getQueue(const Device* pDevice, u32 index) const
		{

			Queue r;
			r.init(pDevice, this, index);
			return r;
		}

		bool QueueFamilies::exists() const
		{
			return m_families != nullptr;
		}

		void QueueFamilies::init(const PhysicalDevice * pDevice)
		{
			if (exists())throw AlreadyInitializedException();

			vkGetPhysicalDeviceQueueFamilyProperties(pDevice->raw(), &m_count, nullptr);
			auto* props = new VkQueueFamilyProperties[m_count];
			vkGetPhysicalDeviceQueueFamilyProperties(pDevice->raw(), &m_count, props);

			m_families = new QueueFamily[m_count];
			for (size_t i = 0; i < m_count; i++)
			{
				m_families[i].init(i, props[i]);
			}
			delete[] props;
		}

		void QueueFamilies::deinit()
		{
			if (!exists())return;

			for (size_t i = 0; i < m_count; i++)
			{
				m_families[i].deinit();
			}

			delete[] m_families;
			m_families = nullptr;
			m_count = 0;
		}

		u32 QueueFamilies::count() const
		{
			if (!exists())throw NotInitializedException();
			return m_count;
		}

		const QueueFamily* QueueFamilies::get(u32 index) const
		{
			if (!exists())throw NotInitializedException();
			return m_families;
		}
		
	}
}