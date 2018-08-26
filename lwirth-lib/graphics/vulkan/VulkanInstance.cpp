#include "../../stdafx.hpp"
#include "VulkanInstance.hpp"

#include <cstring>
#include "../../general/Exceptions.hpp"

namespace lw
{
	namespace VK
	{
		lw::List<const char*> Instance::s_layers =
		{
			"VK_LAYER_LUNARG_standard_validation",
			//"VK_LAYER_LUNARG_assistant_layer",
			//"VK_LAYER_LUNARG_monitor"
			//"VK_LAYER_LUNARG_api_dump" //prints every object creation
		};

		void Instance::create(const std::string& appName, u32 ver_major, u32 ver_minor, u32 ver_patch, u32 vulkanVer)
		{
			if (m_instance != VK_NULL_HANDLE)throw AlreadyCreatedException();

			VkApplicationInfo ai;
			ai.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
			ai.pNext = nullptr;
			ai.pApplicationName = appName.c_str();
			ai.applicationVersion = VK_MAKE_VERSION(ver_major, ver_minor, ver_patch);
			ai.pEngineName = LW_NAME;
			ai.engineVersion = LW_MAKE_VERSION(LW_VER_MAJOR, LW_VER_MINOR, LW_VER_PATCH);
			ai.apiVersion = vulkanVer;

			//layers
			if (!layersAvailable(s_layers))throw VulkanException("Layers are not available");

			//extensions
			u32 glfwExtensionCount = 0;
			const char** glfwExtensions;
			glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);

			VkInstanceCreateInfo ici;
			ici.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
			ici.pNext = nullptr;
			ici.flags = 0;
			ici.pApplicationInfo = &ai;
			ici.enabledLayerCount = s_layers.size();
			ici.ppEnabledLayerNames = s_layers.raw();
			ici.enabledExtensionCount = glfwExtensionCount;
			ici.ppEnabledExtensionNames = glfwExtensions;

			if (vkCreateInstance(&ici, nullptr, &m_instance) != VK_SUCCESS)
			{
				throw VulkanException("failed to create instance");
			}
		}

		void Instance::destroy()
		{
			if (m_instance == VK_NULL_HANDLE)return;

			vkDestroyInstance(m_instance, nullptr);
			m_instance = VK_NULL_HANDLE;
		}

		VkInstance Instance::raw() const
		{
			if (m_instance == VK_NULL_HANDLE)throw NotCreatedException();
			return m_instance;
		}

		VkInstance* Instance::ptr()
		{
			if (m_instance == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_instance;
		}

		bool Instance::layersAvailable(lw::List<const char*>& layers)
		{
			u32 layerCount;
			vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
			lw::List<VkLayerProperties> availableLayers(layerCount);
			vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.raw());

			for (auto layerName : layers)
			{
				bool layerFound = false;
				for (const auto& layerProperties : availableLayers)
				{
					if (std::strcmp(layerName, layerProperties.layerName) == 0)
					{
						layerFound = true;
						break;
					}
				}

				if (!layerFound)
				{
					return false;
				}

			}
			return true;
		}

		




	}
}