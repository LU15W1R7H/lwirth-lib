#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include "VulkanQueues.hpp"

#include "../../memory/List.hpp"

namespace lw
{
	namespace VK
	{
		class PhysicalDeviceContainer;
		class PhysicalDevice;
		class Surface;

		class Device
		{
		private:
			static lw::List<const char*> s_extensions;

			VkDevice			m_device			= VK_NULL_HANDLE;
			Queue				m_graphicsQueue		= {};
			Queue				m_presentQueue		= {};

			VkSurfaceFormatKHR	m_surfaceFormat		= {};
			VkPresentModeKHR	m_presentMode		= {};

			PhysicalDevice*		m_pPhysical			= nullptr;

		public:
			Device() = default;
			~Device();

			bool exists() const;
			void create(const PhysicalDeviceContainer* physicalDevices, const Surface* surface);
			void destroy();

			Queue getQueue(QueueFamily* pFamily, u32 index) const;
			void waitIdle() const;

			VkDevice raw() const;

			PhysicalDevice*				getPhysical() const;
			const Queue*				getGraphicsQueue() const;
			const Queue*				getPresentQueue() const;
			const VkSurfaceFormatKHR&	getSurfaceFormat() const;
			const VkPresentModeKHR&		getPresentMode() const;


		private:
			void chooseSurfaceFormat();
			void choosePresentMode();
			
		};
	}
}