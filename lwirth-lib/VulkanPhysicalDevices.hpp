#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <vector>
#include <map>
#include <string>

#include "VulkanDevice.hpp"
#include "VulkanQueues.hpp"

namespace lw
{
	namespace VK
	{
		class Instance;
		class Surface;

		class API SelectedQueueFamilies
		{
			friend class PhysicalDevice;
		private:
			const QueueFamily* m_pGraphics = nullptr;
			const QueueFamily* m_pPresent = nullptr;

			void find(const PhysicalDevice* pDevice, const Surface* pSurface, const QueueFamilies* pQueueFamilies);
		public:
			bool complete() const;
			std::vector<U32> getIndices() const;

			const QueueFamily* getGraphics() const;
			const QueueFamily* getPresent() const;
		};

		class API PhysicalDevice
		{
			friend class PhysicalDeviceContainer;
		private:
			VkPhysicalDevice						m_device					= VK_NULL_HANDLE;
			const Surface*							m_pSurface					= nullptr;
			VkPhysicalDeviceProperties				m_properties				= {};
			VkPhysicalDeviceFeatures				m_features					= {};
			VkPhysicalDeviceMemoryProperties		m_memoryProperties			= {};
			std::vector<VkSurfaceFormatKHR>			m_surfaceFormats			= {};
			std::vector<VkPresentModeKHR>			m_presentModes				= {};
			std::vector<VkExtensionProperties>		m_extensionProperties		= {};
			QueueFamilies							m_queueFamilies				= {};
			SelectedQueueFamilies					m_selectedQueueFamilies		= {};
			U32										m_rating					= 0;
			
			const static std::vector<std::string>	s_extensions;

		public:
			PhysicalDevice();
			~PhysicalDevice();


			VkPhysicalDevice raw() const;

			//getter
			const VkPhysicalDeviceProperties& properties() const { return m_properties; }
			const VkPhysicalDeviceFeatures& features() const { return m_features; }
			const VkPhysicalDeviceMemoryProperties& memoryProperties() const { return m_memoryProperties; }
			VkSurfaceCapabilitiesKHR surfaceCapabilities() const;
			const std::vector<VkSurfaceFormatKHR>& surfaceFormats() const { return m_surfaceFormats; }
			const std::vector<VkPresentModeKHR>& presentModes() const { return m_presentModes; }
			const std::vector<VkExtensionProperties>& extensionProperties() const { return m_extensionProperties; }
			const SelectedQueueFamilies* queueFamiles() const { return &m_selectedQueueFamilies; }

		private:
			void init(VkPhysicalDevice device, const Surface* pSurface);

			void queryProperties();
			bool checkExtensionSupport();
			void rateSuitability();
		};

		class API PhysicalDeviceContainer
		{
		private:
			std::vector<PhysicalDevice> m_devices = {};
			PhysicalDevice* m_pBest = nullptr;
		public:
			PhysicalDeviceContainer() {}
			~PhysicalDeviceContainer() {}

			void init(const Instance* instance, const Surface* surface);

			PhysicalDevice* getBest() const { return m_pBest; }
		};

		

	}
}