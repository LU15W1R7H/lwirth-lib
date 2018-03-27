#include "stdafx.hpp"
#include "VulkanSurface.hpp"

#include "Exceptions.hpp"
#include "Frame.hpp"
#include "VulkanInstance.hpp"

namespace lw
{
	namespace VK
	{
	
		Surface::~Surface()
		{
			if (m_surface != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void Surface::create(const Instance* instance, const Frame* frame)
		{
			if (m_surface != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pInstance = instance;

			if (glfwCreateWindowSurface(m_pInstance->raw(), frame->raw(), nullptr, &m_surface) != VK_SUCCESS)
			{
				throw VulkanException("failed to create surface");
			}
		}

		void Surface::destroy()
		{
			if (m_surface == VK_NULL_HANDLE)return;

			vkDestroySurfaceKHR(m_pInstance->raw(), m_surface, nullptr);
			m_surface = VK_NULL_HANDLE;
		}

		VkSurfaceKHR Surface::raw() const
		{
			if (m_surface == VK_NULL_HANDLE)throw NotCreatedException();

			return m_surface;
		}

		VkSurfaceKHR* Surface::ptr()
		{
			if (m_surface == VK_NULL_HANDLE)throw NotCreatedException();

			return &m_surface;
		}
	}
}
