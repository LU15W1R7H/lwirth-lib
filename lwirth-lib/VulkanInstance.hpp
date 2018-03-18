#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <string>
#include <vector>

namespace lw
{
	namespace VK
	{
		class API Instance
		{
		private:
			VkInstance m_instance = VK_NULL_HANDLE;

			static std::vector<const char*> s_layers;

		public:
			Instance();
			~Instance();

			void create(std::string& appName, U32 ver_major, U32 ver_minor, U32 ver_patch);
			void create(std::string&& appName, U32 ver_major, U32 ver_minor, U32 ver_patch) { create(appName, ver_major, ver_minor, ver_patch); }
			void destroy();

			VkInstance raw() const;
			VkInstance* ptr();

		private:
			static bool layersAvailable(std::vector<const char*>& layers);
		};

	}
}