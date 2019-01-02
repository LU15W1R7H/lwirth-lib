#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

namespace lw
{
	class Frame;

	namespace VK
	{
		class Instance;

		class Surface
		{
		private:
			VkSurfaceKHR m_surface	= VK_NULL_HANDLE;
			const Instance* m_pInstance	= nullptr;

		public:
			Surface() = default;
			~Surface();

			void create(const Instance* instance, const Frame* frame);
			void destroy();

			VkSurfaceKHR raw() const;
			VkSurfaceKHR* ptr();

		private:
		};

	}
}