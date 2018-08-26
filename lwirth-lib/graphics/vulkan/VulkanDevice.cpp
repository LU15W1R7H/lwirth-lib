#include "../../stdafx.hpp"
#include "VulkanDevice.hpp"

#include "../../general/Exceptions.hpp"
#include "VulkanPhysicalDevices.hpp"
#include "VulkanSurface.hpp"
#include "VulkanQueues.hpp"

#include <set>

namespace lw
{
	namespace VK
	{
		lw::List<const char*> Device::s_extensions =
		{
			VK_KHR_SWAPCHAIN_EXTENSION_NAME
		};

		Device::~Device()
		{
		}

		bool Device::exists() const
		{
			return m_device != VK_NULL_HANDLE;
		}

		void Device::create(const PhysicalDeviceContainer* physicalDevices, const Surface* surface)
		{
			if (exists())throw AlreadyCreatedException();

			m_pPhysical = physicalDevices->getBest();


			lw::List<u32> temp = m_pPhysical->queueFamiles()->getIndices();
			std::set<u32> familyIndices;
			for (unsigned int i : temp)
			{
				familyIndices.insert(i);
			}

			lw::List<VkDeviceQueueCreateInfo> queueInfos;
			float queuePriority = 1.f;
			for (u32 index : familyIndices)
			{
				VkDeviceQueueCreateInfo info;
				info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
				info.pNext = nullptr;
				info.flags = 0;
				info.queueFamilyIndex = index;
				info.queueCount = 1;
				info.pQueuePriorities = &queuePriority;
				queueInfos.push(info);
			}
			
			VkPhysicalDeviceFeatures usedFeatures = {};
			usedFeatures.samplerAnisotropy = VK_TRUE;
			usedFeatures.fillModeNonSolid = VK_TRUE;

			VkDeviceCreateInfo deviceInfo;
			deviceInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
			deviceInfo.pNext = nullptr;
			deviceInfo.flags = 0;
			deviceInfo.queueCreateInfoCount = queueInfos.size();
			deviceInfo.pQueueCreateInfos = queueInfos.raw();
			deviceInfo.enabledLayerCount = 0;	//Validation layers on device-level are deprecated.
			deviceInfo.ppEnabledLayerNames = nullptr;
			deviceInfo.enabledExtensionCount = s_extensions.size();
			deviceInfo.ppEnabledExtensionNames = s_extensions.raw();
			deviceInfo.pEnabledFeatures = &usedFeatures;

			if (vkCreateDevice(m_pPhysical->raw(), &deviceInfo, nullptr, &m_device) != VK_SUCCESS)
			{
				throw VulkanException("failed to create logical device");
			}

			
			m_graphicsQueue.init(this, m_pPhysical->queueFamiles()->getGraphics(), 0);
			m_presentQueue.init(this, m_pPhysical->queueFamiles()->getPresent(), 0);

			chooseSurfaceFormat();
			choosePresentMode();
		}

		void Device::destroy()
		{
			if (!exists())return;

			vkDestroyDevice(m_device, nullptr);
			m_device = VK_NULL_HANDLE;
			m_graphicsQueue.clear();
			m_presentQueue.clear();
			m_surfaceFormat = {};
			m_presentMode = {};
			m_pPhysical = nullptr;
		}

		Queue Device::getQueue(QueueFamily* pFamily, u32 index) const
		{
			if (m_device == VK_NULL_HANDLE)throw NotCreatedException();
			Queue r;
			r.init(this, pFamily, index);
		}

		void Device::waitIdle() const
		{
			if (!exists())throw NotCreatedException();
			vkDeviceWaitIdle(m_device);
		}

		VkDevice Device::raw() const
		{
			if (!exists())throw NotCreatedException();
			return m_device;
		}

		PhysicalDevice* Device::getPhysical() const
		{
			if (!exists())throw NotCreatedException();
			return m_pPhysical;
		}

		const Queue* Device::getGraphicsQueue() const
		{
			if (!exists())throw NotCreatedException();
			return &m_graphicsQueue;
		}

		const Queue* Device::getPresentQueue() const
		{
			if (!exists())throw NotCreatedException();
			return &m_presentQueue;
		}

		const VkSurfaceFormatKHR& Device::getSurfaceFormat() const
		{
			if (!exists())throw NotCreatedException();
			return m_surfaceFormat;
		}

		const VkPresentModeKHR& Device::getPresentMode() const
		{
			if (!exists())throw NotCreatedException();
			return m_presentMode;
		}

		void Device::chooseSurfaceFormat()
		{
			if (m_pPhysical->surfaceFormats().size() == 1 && m_pPhysical->surfaceFormats()[0].format == VK_FORMAT_UNDEFINED)
			{
				m_surfaceFormat = { VK_FORMAT_B8G8R8A8_UNORM, VK_COLOR_SPACE_SRGB_NONLINEAR_KHR };
				return;
			}

			for (const auto& f : m_pPhysical->surfaceFormats())
			{
				if (f.format == VK_FORMAT_B8G8R8A8_UNORM && f.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
				{
					m_surfaceFormat = f;
					return;
				}
			}

			m_surfaceFormat = m_pPhysical->surfaceFormats()[0];
		}

		void Device::choosePresentMode()
		{
			VkPresentModeKHR bestMode = VK_PRESENT_MODE_FIFO_KHR;

			for (const auto& mode : m_pPhysical->presentModes())
			{
				if (mode == VK_PRESENT_MODE_MAILBOX_KHR)
				{
					bestMode = mode;
					break;
				}
				else if (mode == VK_PRESENT_MODE_IMMEDIATE_KHR)
				{
					bestMode = mode;
				}
			}
			m_presentMode = bestMode;
		}

		
	}
}