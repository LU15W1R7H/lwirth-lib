#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "DynamicArray.hpp"

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
			
			bool m_created = false;
			bool m_init = false;

			VkPipelineInputAssemblyStateCreateInfo	m_piasci			= {};
			DynamicArray<VkViewport>				m_viewports			= {};
			DynamicArray<VkRect2D>					m_scissors			= {};
			VkPipelineRasterizationStateCreateInfo	m_prsci				= {};
			VkPipelineMultisampleStateCreateInfo	m_pmsci				= {};
			VkPipelineColorBlendAttachmentState		m_cbas				= {};
			VkPipelineColorBlendStateCreateInfo		m_pcbsci			= {};
			DynamicArray<VkDynamicState>			m_dynamicStates		= {};
			VkPipelineDepthStencilStateCreateInfo	m_pdssci			= {};


			DynamicArray<VkPushConstantRange>				m_pushConstantRanges			= {};
			DynamicArray<VkVertexInputBindingDescription>	m_vertexBindingDescriptions		= {};
			DynamicArray<VkVertexInputAttributeDescription>	m_vertexAttributeDescriptions	= {};
			DynamicArray<VkDescriptorSetLayout>				m_descriptorSetLayouts			= {};

			const Device* m_pDevice = nullptr;

		public:
			Pipeline() = default;
			~Pipeline();

			void init(U32 screenWidth, U32 screenHeight);
			void create(const Device* pDevice, const RenderPass* pRenderPass, const VertexShader* pVertexShader, const FragmentShader* pFragmentShader);
			void destroy();

			VkPipeline raw() const;
			VkPipelineLayout getLayout() const;

			Pipeline& setTopology(VkPrimitiveTopology topology);
			Pipeline& setPrimitiveRestartEnable(bool enable);
			Pipeline& addViewport(const VkViewport& viewport, const VkRect2D& scissor);
			Pipeline& setDepthClambEnable(bool enable);
			Pipeline& setRasterizerDiscardEnable(bool enable);
			Pipeline& setPolygonMode(VkPolygonMode mode);
			Pipeline& setCulling(VkCullModeFlags mode, VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE);
			Pipeline& setDepthBias(bool enable, F32 constFactor, F32 clamp, F32 slopeFactor);
			Pipeline& setLineWidth(F32 width);
			Pipeline& addDynamicState(VkDynamicState state);
			Pipeline& setColorBlending(const VkPipelineColorBlendAttachmentState& attachment, bool enableLogigOp, VkLogicOp logicOp, F32 blendConst0, F32 blendConst1, F32 blendConst2, F32 blendConst3);
			Pipeline& addVertexBinding(const VkVertexInputBindingDescription& vibd);
			Pipeline& addVertexBinding(U32 binding, U32 stride, VkVertexInputRate inputRate);
			Pipeline& addVertexDescription(const VkVertexInputAttributeDescription& viad);
			Pipeline& addVertexDescription(U32 location, U32 binding, VkFormat format, U32 offset);
			Pipeline& setDepthStencil(bool enableDepthTest, bool enableDepthWrite, VkCompareOp depthCompareOp, bool enableDepthBoundsTest, bool enableStencilTest, VkStencilOpState front, VkStencilOpState back, F32 minDepthBounds, F32 maxDepthBounds);
			Pipeline& addDesciptorSetLayout(const VkDescriptorSetLayout& dsl);
			Pipeline& addPushConstantRange(const VkPushConstantRange& pcr);
			Pipeline& addPushConstantRange(VkShaderStageFlags stageFlags, U32 offset, U32 size);

			//#TODO: setMultisample()
			//#TODO: setTesselationState()

		private:
			void checkInitNotCreated();

			

		};

	}
}