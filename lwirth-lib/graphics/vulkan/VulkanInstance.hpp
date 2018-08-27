#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>
#include "../../memory/List.hpp"

namespace lw
{
	namespace VK
	{
		class Instance
		{
		private:
			VkInstance m_instance = VK_NULL_HANDLE;

			static lw::List<const char*> s_layers;

		public:
			Instance() = default;
			~Instance() = default;
			DISABLE_COP_MOV(Instance);
			
			void create(const std::string& appName, u32 ver_major, u32 ver_minor, u32 ver_patch, u32 vulkanVer);
			void create(std::string&& appName, u32 ver_major, u32 ver_minor, u32 ver_patch, u32 vulkanVer) { create(appName, ver_major, ver_minor, ver_patch, vulkanVer); }
			void destroy();

			VkInstance raw() const;
			VkInstance* ptr();

		private:
			static bool layersAvailable(lw::List<const char*>& layers);
		};

	}
}