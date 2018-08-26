#include "../../stdafx.hpp"
#include "VulkanSemaphore.hpp"

#include "../../general/Exceptions.hpp"
#include "VulkanDevice.hpp"

namespace lw
{
	namespace VK
	{

		Semaphore::~Semaphore()
		{
		}

		void Semaphore::create(Device* pDevice)
		{
			if (m_semaphore != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;

			VkSemaphoreCreateInfo sci;
			sci.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
			sci.pNext = nullptr;
			sci.flags = 0;

			if (vkCreateSemaphore(m_pDevice->raw(), &sci, nullptr, &m_semaphore) != VK_SUCCESS)
			{
				throw VulkanException("failed to create semaphore");
			}
		}

		void Semaphore::destroy()
		{
			if (m_semaphore == VK_NULL_HANDLE)return;

			vkDestroySemaphore(m_pDevice->raw(), m_semaphore, nullptr);
			m_semaphore = VK_NULL_HANDLE;
			m_pDevice = nullptr;
		}

		VkSemaphore Semaphore::raw() const
		{
			if (m_semaphore == VK_NULL_HANDLE)throw NotCreatedException();
			return m_semaphore;
		}

		VkSemaphore* Semaphore::ptr()
		{
			if (m_semaphore == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_semaphore;
		}

		const VkSemaphore * Semaphore::ptr() const
		{
			if (m_semaphore == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_semaphore;
		}


	}
}