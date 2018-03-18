#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <string>

namespace lw
{
	namespace VK
	{
		class Device;

		class API VertexShader
		{
		private:
			VkShaderModule m_module = VK_NULL_HANDLE;

			const Device* m_pDevice = nullptr;

		public:
			VertexShader();
			~VertexShader();

			void create(const Device* device, const std::string& filePath);
			void create(const Device* device, const std::string&& filePath) { VertexShader::create(device, filePath); }
			void destroy();

			VkShaderModule raw() const;
			VkPipelineShaderStageCreateInfo getStageInfo() const;

		};

		class API FragmentShader
		{
		private:
			VkShaderModule m_module = VK_NULL_HANDLE;

			const Device* m_pDevice = nullptr;

		public:
			FragmentShader();
			~FragmentShader();

			void create(const Device* device, const std::string& filePath);
			void create(const Device* device, const std::string&& filePath) { FragmentShader::create(device, filePath); }
			void destroy();

			VkShaderModule raw() const;
			VkPipelineShaderStageCreateInfo getStageInfo() const;

		};

	}
}