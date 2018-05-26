#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "List.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class RenderPass;
		class VertexShader;
		class FragmentShader;
		class CommandBuffer;

		class Pipeline
		{
		private:
			VkPipeline			m_pipeline			= VK_NULL_HANDLE;
			VkPipelineLayout	m_layout			= VK_NULL_HANDLE;
			
			bool m_created = false;
			bool m_init = false;

			VkPipelineInputAssemblyStateCreateInfo	m_piasci			= {};
			List<VkViewport>				m_viewports			= {};
			List<VkRect2D>					m_scissors			= {};
			VkPipelineRasterizationStateCreateInfo	m_prsci				= {};
			VkPipelineMultisampleStateCreateInfo	m_pmsci				= {};
			VkPipelineColorBlendAttachmentState		m_cbas				= {};
			VkPipelineColorBlendStateCreateInfo		m_pcbsci			= {};
			List<VkDynamicState>			m_dynamicStates		= {};
			VkPipelineDepthStencilStateCreateInfo	m_pdssci			= {};


			List<VkPushConstantRange>				m_pushConstantRanges			= {};
			List<VkVertexInputBindingDescription>	m_vertexBindingDescriptions		= {};
			List<VkVertexInputAttributeDescription>	m_vertexAttributeDescriptions	= {};
			List<VkDescriptorSetLayout>				m_descriptorSetLayouts			= {};

			const Device* m_pDevice = nullptr;

		public:
			Pipeline() = default;
			~Pipeline();

			void init(u32 screenWidth, u32 screenHeight);
			void create(const Device* pDevice, const RenderPass* pRenderPass, const VertexShader* pVertexShader, const FragmentShader* pFragmentShader);
			void destroy();

			VkPipeline raw() const;
			VkPipelineLayout getLayout() const;

			void bind(const CommandBuffer& cmd, VkPipelineBindPoint bindPoint) const;

			Pipeline& setTopology(VkPrimitiveTopology topology);
			Pipeline& setPrimitiveRestartEnable(bool enable);
			Pipeline& addViewport(const VkViewport& viewport, const VkRect2D& scissor);
			Pipeline& setDepthClambEnable(bool enable);
			Pipeline& setRasterizerDiscardEnable(bool enable);
			Pipeline& setPolygonMode(VkPolygonMode mode);
			Pipeline& setCulling(VkCullModeFlags mode, VkFrontFace frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE);
			Pipeline& setDepthBias(bool enable, f32 constFactor, f32 clamp, f32 slopeFactor);
			Pipeline& setLineWidth(f32 width);
			Pipeline& addDynamicState(VkDynamicState state);
			Pipeline& setColorBlending(const VkPipelineColorBlendAttachmentState& attachment, bool enableLogigOp, VkLogicOp logicOp, f32 blendConst0, f32 blendConst1, f32 blendConst2, f32 blendConst3);
			Pipeline& addVertexBinding(const VkVertexInputBindingDescription& vibd);
			Pipeline& addVertexBinding(u32 binding, u32 stride, VkVertexInputRate inputRate);
			Pipeline& addVertexDescription(const VkVertexInputAttributeDescription& viad);
			Pipeline& addVertexDescription(u32 location, u32 binding, VkFormat format, u32 offset);
			Pipeline& setDepthStencil(bool enableDepthTest, bool enableDepthWrite, VkCompareOp depthCompareOp, bool enableDepthBoundsTest, bool enableStencilTest, VkStencilOpState front, VkStencilOpState back, f32 minDepthBounds, f32 maxDepthBounds);
			//Pipeline& setMultisampling(VkSampleCountFlagBits sampleCount, bool enableSampleShading, F32 minSampleShading, VkSampleMask sampleMask, bool enableAlphaToCoverage, bool enableAlphaToOne);
			Pipeline& addDesciptorSetLayout(const VkDescriptorSetLayout& dsl);
			Pipeline& addPushConstantRange(const VkPushConstantRange& pcr);
			Pipeline& addPushConstantRange(VkShaderStageFlags stageFlags, u32 offset, u32 size);

			//#TODO: setTesselationState()



		private:
			void checkInitNotCreated();

			

		};

	}
}