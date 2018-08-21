#pragma once

#include "../../Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

namespace lw
{
	namespace VK
	{
		class Device;

		class Semaphore
		{
		private:
			VkSemaphore	m_semaphore	= VK_NULL_HANDLE;
			Device*		m_pDevice	= nullptr;

		public:
			Semaphore() = default;
			~Semaphore();

			void create(Device* pDevice);
			void destroy();

			VkSemaphore raw() const;
			VkSemaphore* ptr();
			const VkSemaphore* ptr() const;
		};

		

	}
}