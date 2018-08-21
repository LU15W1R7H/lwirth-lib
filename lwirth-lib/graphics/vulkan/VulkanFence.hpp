#pragma once

#include "../../Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <limits>


namespace lw
{
	namespace VK
	{
		class Device;

		class Fence
		{
		private:
			VkFence	m_fence = VK_NULL_HANDLE;

			const Device* m_pDevice = nullptr;

		public:
			Fence() = default;
			~Fence();

			void create(const Device* pDevice);
			void destroy();

			void wait(u64 timeout = std::numeric_limits<u64>::max());
			void reset();

			VkFence raw() const;
		};

	}
}
