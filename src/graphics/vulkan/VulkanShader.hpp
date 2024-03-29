#pragma once

#include "../../Standard.hpp"

#include <vulkan/vulkan.h>
#include <GLFW/glfw3.h>

#include <string>

namespace lw
{
	namespace VK
	{
		class Device;

		class VertexShader
		{
		private:
			VkShaderModule m_module = VK_NULL_HANDLE;

			const Device* m_pDevice = nullptr;

		public:
			VertexShader() = default;
			~VertexShader();

			void create(const Device* device, const std::string& filePath);
			void create(const Device* device, const std::string&& filePath) { VertexShader::create(device, filePath); }
			void destroy();

			VkShaderModule raw() const;
			VkPipelineShaderStageCreateInfo getStageInfo() const;

		};

		class FragmentShader
		{
		private:
			VkShaderModule m_module = VK_NULL_HANDLE;

			const Device* m_pDevice = nullptr;

		public:
			FragmentShader() = default;
			~FragmentShader();

			void create(const Device* device, const std::string& filePath);
			void create(const Device* device, const std::string&& filePath) { FragmentShader::create(device, filePath); }
			void destroy();

			VkShaderModule raw() const;
			VkPipelineShaderStageCreateInfo getStageInfo() const;

		};

	}
}