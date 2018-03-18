#include "stdafx.hpp"
#include "VulkanPhysicalDevices.hpp"

#include "Exceptions.hpp"
#include "VulkanInstance.hpp"
#include "VulkanSurface.hpp"
#include <set>
#include <map>


namespace lw
{
	namespace VK
	{
		const std::vector<std::string> PhysicalDevice::s_extensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		PhysicalDevice::PhysicalDevice()
		{	
		}

		PhysicalDevice::~PhysicalDevice()
		{
		}

		void PhysicalDevice::init(VkPhysicalDevice device, const Surface* pSurface)
		{
			if (m_device != VK_NULL_HANDLE)throw AlreadyInitializedException();

			m_device = device;
			m_pSurface = pSurface;
			m_rating = 0;

			queryProperties();
			m_selectedQueueFamilies.find(this, pSurface, &m_queueFamilies);
			checkExtensionSupport();
			rateSuitability();
		}

		VkPhysicalDevice PhysicalDevice::raw() const
		{
			if (m_device == VK_NULL_HANDLE)throw NotInitializedException();

			return m_device;
		}

		void PhysicalDevice::queryProperties()
		{
			vkGetPhysicalDeviceProperties(m_device, &m_properties);
			vkGetPhysicalDeviceFeatures(m_device, &m_features);
			vkGetPhysicalDeviceMemoryProperties(m_device, &m_memoryProperties);

			m_queueFamilies.init(this);

			U32 amountSurfaceFormats;
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_pSurface->raw(), &amountSurfaceFormats, nullptr);
			m_surfaceFormats.resize(amountSurfaceFormats);
			vkGetPhysicalDeviceSurfaceFormatsKHR(m_device, m_pSurface->raw(), &amountSurfaceFormats, m_surfaceFormats.data());

			U32 amountPresentModes;
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_pSurface->raw(), &amountPresentModes, nullptr);
			m_presentModes.resize(amountPresentModes);
			vkGetPhysicalDeviceSurfacePresentModesKHR(m_device, m_pSurface->raw(), &amountPresentModes, m_presentModes.data());

			U32 amountExtensionProperties;
			vkEnumerateDeviceExtensionProperties(m_device, nullptr, &amountExtensionProperties, nullptr);
			m_extensionProperties.resize(amountExtensionProperties);
			vkEnumerateDeviceExtensionProperties(m_device, nullptr, &amountExtensionProperties, m_extensionProperties.data());
		}

		bool PhysicalDevice::checkExtensionSupport()
		{
			std::set<std::string> requiredExts(s_extensions.begin(), s_extensions.end());

			for (const auto& e : m_extensionProperties)
			{
				requiredExts.erase(e.extensionName);
			}

			return requiredExts.empty();
		}

		void PhysicalDevice::rateSuitability()
		{
			if (m_properties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU)
			{
				m_rating += 1000;
			}

			m_rating += m_properties.limits.maxImageDimension2D;

			//is 100% required
			{
				if (!m_selectedQueueFamilies.complete())m_rating = 0;
				if (!checkExtensionSupport())m_rating = 0;
				if (m_surfaceFormats.empty() || m_presentModes.empty())m_rating = 0;
				if (!m_features.samplerAnisotropy)m_rating = 0;
			}
		}

		VkSurfaceCapabilitiesKHR PhysicalDevice::surfaceCapabilities() const
		{
			VkSurfaceCapabilitiesKHR surfaceCapabilities;
			vkGetPhysicalDeviceSurfaceCapabilitiesKHR(m_device, m_pSurface->raw(), &surfaceCapabilities);
			return surfaceCapabilities;
		}
		

		
		void PhysicalDeviceContainer::init(const Instance* instance, const Surface* surface)
		{
			{
				VkPhysicalDevice* physicalDevices;
				U32 amountDevices;
				vkEnumeratePhysicalDevices(instance->raw(), &amountDevices, nullptr);
				physicalDevices = new VkPhysicalDevice[amountDevices];
				vkEnumeratePhysicalDevices(instance->raw(), &amountDevices, physicalDevices);

				m_devices.resize(amountDevices);

				for (U32 i = 0; i < amountDevices; i++)
				{
					m_devices[i].init(physicalDevices[i], surface);
				}
				delete[] physicalDevices;
			}

			typedef U32 PhysicalDeviceIndex;
			std::multimap<U32, PhysicalDeviceIndex> candidates;

			for (U32 i = 0; i < m_devices.size(); i++)
			{
				candidates.insert(std::make_pair(m_devices[i].m_rating, i));
			}

			if (candidates.rbegin()->first > 0)
			{
				m_pBest = &m_devices[candidates.rbegin()->second];
			}
			else
			{
				throw VulkanException("failed to find a suitable GPU");
			}
		}

		bool SelectedQueueFamilies::complete() const
		{
			return (m_pGraphics != nullptr && m_pPresent != nullptr);
		}

		void SelectedQueueFamilies::find(const PhysicalDevice* pDevice, const Surface* pSurface, const QueueFamilies* pQueueFamilies)
		{
			for (U32 i = 0; i < pQueueFamilies->count(); i++)
			{
				auto& f = pQueueFamilies->get(i)->getProperties();;
				if (f.queueCount > 0 && f.queueFlags & VK_QUEUE_GRAPHICS_BIT)
				{
					m_pGraphics = pQueueFamilies->get(i);
				}

				VkBool32 presentSupport = VK_FALSE;
				vkGetPhysicalDeviceSurfaceSupportKHR(pDevice->raw(), i, pSurface->raw(), &presentSupport);
				if (f.queueCount > 0 && presentSupport)
				{
					m_pPresent = pQueueFamilies->get(i);
				}

				if (complete())return;
			}
		}

		std::vector<U32> SelectedQueueFamilies::getIndices() const
		{
			if (!complete())throw NotInitializedException();
			std::vector<U32> r =
			{
				m_pGraphics->getIndex(),
				m_pPresent->getIndex()
			};
			return r;
		}

		const QueueFamily* SelectedQueueFamilies::getGraphics() const
		{
			if (!complete())throw NotInitializedException();
			return m_pGraphics;
		}

		const QueueFamily * SelectedQueueFamilies::getPresent() const
		{
			if (!complete())throw NotInitializedException();
			return m_pPresent;
		}

}
}