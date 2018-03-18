#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include <vector>

namespace lw
{
	namespace VK
	{
		class Device;
		class RenderPass;
		class VertexShader;
		class FragmentShader;

		class API Pipeline
		{
		private:
			VkPipeline			m_pipeline			= VK_NULL_HANDLE;
			VkPipelineLayout	m_layout			= VK_NULL_HANDLE;
			
			bool											m_useDepthBuffer = false;
			std::vector<VkPushConstantRange>				m_pushConstantRanges			= {};
			std::vector<VkVertexInputBindingDescription>	m_vertexBindingDescriptions		= {};
			std::vector<VkVertexInputAttributeDescription>	m_vertexAttributeDescriptions	= {};
			std::vector<VkDescriptorSetLayout>				m_descriptorSetLayouts			= {};

			const Device* m_pDevice = nullptr;

		public:
			Pipeline();
			~Pipeline();

			void create(const Device* pDevice, const RenderPass* pRenderPass, const VertexShader* pVertexShader, const FragmentShader* pFragmentShader, U32 screenWidth, U32 screenHeight);
			void destroy();

			VkPipeline raw() const;
			VkPipelineLayout getLayout() const;

			void addVertexBinding(const VkVertexInputBindingDescription* vibd);
			void addVertexBinding(U32 binding, U32 stride, VkVertexInputRate inputRate);
			
			void addVertexDescription(const VkVertexInputAttributeDescription* viad);
			void addVertexDescription(U32 location, U32 binding, VkFormat format, U32 offset);

			void addDesciptorSetLayout(const VkDescriptorSetLayout* dsl);
			
			void addPushConstantRange(const VkPushConstantRange* pcr);
			void addPushConstantRange(VkShaderStageFlags stageFlags, U32 offset, U32 size);

			void enableDepthBuffer();
		};

	}
}