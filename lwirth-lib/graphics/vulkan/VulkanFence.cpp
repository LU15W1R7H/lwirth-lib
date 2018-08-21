#include "../../stdafx.hpp"
#include "VulkanFence.hpp"

#include "VulkanDevice.hpp"
#include "../../general/Exceptions.hpp"

namespace lw
{
	namespace VK
	{

		Fence::~Fence()
		{
			if (m_fence != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void Fence::create(const Device* pDevice)
		{
			if (m_fence != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;

			VkFenceCreateInfo fci;
			fci.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
			fci.pNext = nullptr;
			fci.flags = 0;

			if (vkCreateFence(m_pDevice->raw(), &fci, nullptr, &m_fence) != VK_SUCCESS)
			{
				throw VulkanException("failed to create fence");
			}
		}

		void Fence::destroy()
		{
			if (m_fence == VK_NULL_HANDLE)return;

			vkDestroyFence(m_pDevice->raw(), m_fence, nullptr);
			m_pDevice = nullptr;
			m_fence = VK_NULL_HANDLE;
		}

		void Fence::wait(u64 timeout)
		{
			if (m_fence == VK_NULL_HANDLE)throw NotCreatedException();

			if (vkWaitForFences(m_pDevice->raw(), 1, &m_fence, VK_TRUE, timeout) != VK_SUCCESS)
			{
				throw VulkanException("failed to wait for fence");
			}
			Fence::reset();
		}

		void Fence::reset()
		{
			if (m_fence == VK_NULL_HANDLE)throw NotCreatedException();

			if (vkResetFences(m_pDevice->raw(), 1, &m_fence) != VK_SUCCESS)
			{
				throw VulkanException("failed to reset fence");
			}
		}
		VkFence Fence::raw() const
		{
			if (m_fence == VK_NULL_HANDLE)throw NotCreatedException();
			return m_fence;
		}
	}
}
