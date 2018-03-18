#include "stdafx.hpp"
#include "VulkanShader.hpp"

#include "Exceptions.hpp"
#include "VulkanDevice.hpp"
#include "FileManager.hpp"

namespace lw
{
	namespace VK
	{
		VertexShader::VertexShader()
		{
		}

		VertexShader::~VertexShader()
		{
			if (m_module != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void VertexShader::create(const Device* device, const std::string& filePath)
		{
			if (m_module != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = device;

			auto code = readBinFile(filePath);
			VkShaderModuleCreateInfo moduleInfo;
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleInfo.pNext = nullptr;
			moduleInfo.flags = 0;
			moduleInfo.codeSize = code.size();
			moduleInfo.pCode = reinterpret_cast<const U32*>(code.data());
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

			if (vkCreateShaderModule(m_pDevice->raw(), &moduleInfo, nullptr, &m_module) != VK_SUCCESS)
			{
				throw VulkanException("failed to create shader module");
			}
		}

		void VertexShader::destroy()
		{
			if (m_module == VK_NULL_HANDLE)return;

			vkDestroyShaderModule(m_pDevice->raw(), m_module, nullptr);
			m_module = VK_NULL_HANDLE;
		}

		VkShaderModule VertexShader::raw() const
		{
			if (m_module == VK_NULL_HANDLE)throw NotCreatedException();
			return m_module;
		}

		VkPipelineShaderStageCreateInfo VertexShader::getStageInfo() const
		{
			if (m_module == VK_NULL_HANDLE)throw NotCreatedException();

			VkPipelineShaderStageCreateInfo pssci;
			pssci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pssci.pNext = nullptr;
			pssci.flags = 0;
			pssci.stage = VK_SHADER_STAGE_VERTEX_BIT;
			pssci.module = m_module;
			pssci.pName = "main";
			pssci.pSpecializationInfo = nullptr;
			return pssci;
		}

		//FragmentShader

		FragmentShader::FragmentShader()
		{
		}

		FragmentShader::~FragmentShader()
		{
			if (m_module != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void FragmentShader::create(const Device* device, const std::string& filePath)
		{
			if (m_module != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = device;

			auto code = readBinFile(filePath);
			VkShaderModuleCreateInfo moduleInfo;
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
			moduleInfo.pNext = nullptr;
			moduleInfo.flags = 0;
			moduleInfo.codeSize = code.size();
			moduleInfo.pCode = reinterpret_cast<const U32*>(code.data());
			moduleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;

			if (vkCreateShaderModule(m_pDevice->raw(), &moduleInfo, nullptr, &m_module) != VK_SUCCESS)
			{
				throw VulkanException("failed to create shader module");
			}
		}

		void FragmentShader::destroy()
		{
			if (m_module == VK_NULL_HANDLE)return;

			vkDestroyShaderModule(m_pDevice->raw(), m_module, nullptr);
			m_module = VK_NULL_HANDLE;
		}

		VkShaderModule FragmentShader::raw() const
		{
			if (m_module == VK_NULL_HANDLE)throw NotCreatedException();
			return m_module;
		}

		VkPipelineShaderStageCreateInfo FragmentShader::getStageInfo() const
		{
			if (m_module == VK_NULL_HANDLE)throw NotCreatedException();

			VkPipelineShaderStageCreateInfo pssci;
			pssci.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
			pssci.pNext = nullptr;
			pssci.flags = 0;
			pssci.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
			pssci.module = m_module;
			pssci.pName = "main";
			pssci.pSpecializationInfo = nullptr;
			return pssci;
		}
			


	}
}
