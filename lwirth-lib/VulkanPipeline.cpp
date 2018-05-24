#include "stdafx.hpp"
#include "VulkanPipeline.hpp"


#include "Exceptions.hpp"
#include "VulkanShader.hpp"
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "FileManager.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	namespace VK
	{

		Pipeline::~Pipeline()
		{
			if (m_pipeline != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void Pipeline::init(u32 screenWidth, u32 screenHeight)
		{
			if (m_init)throw AlreadyInitializedException();

			m_piasci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			m_piasci.pNext = nullptr;
			m_piasci.flags = 0;
			m_piasci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			m_piasci.primitiveRestartEnable = VK_FALSE;

			{
				VkViewport viewport;
				viewport.x = 0.f;
				viewport.y = 0.f;
				viewport.width = static_cast<f32>(screenWidth);
				viewport.height = static_cast<f32>(screenHeight);
				viewport.minDepth = 0.f;
				viewport.maxDepth = 1.f;
				m_viewports.push(viewport);

				VkRect2D scissor;
				scissor.offset = { 0, 0 };
				scissor.extent = { screenWidth, screenHeight };
				m_scissors.push(scissor);
			}

			m_prsci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			m_prsci.pNext = nullptr;
			m_prsci.flags = 0;
			m_prsci.depthClampEnable = VK_FALSE;
			m_prsci.rasterizerDiscardEnable = VK_FALSE;
			m_prsci.polygonMode = VK_POLYGON_MODE_FILL;
			m_prsci.cullMode = VK_CULL_MODE_NONE; //SHOULD BE BACK_BIT
			m_prsci.frontFace = VK_FRONT_FACE_CLOCKWISE;
			m_prsci.depthBiasEnable = VK_FALSE;
			m_prsci.depthBiasConstantFactor = 0.f;
			m_prsci.depthBiasClamp = 0.f;
			m_prsci.depthBiasSlopeFactor = 0.f;
			m_prsci.lineWidth = 1.0f;

			m_pmsci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			m_pmsci.pNext = nullptr;
			m_pmsci.flags = 0;
			m_pmsci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			m_pmsci.sampleShadingEnable = VK_FALSE;
			m_pmsci.minSampleShading = 1.f;
			m_pmsci.pSampleMask = nullptr;
			m_pmsci.alphaToCoverageEnable = VK_FALSE;
			m_pmsci.alphaToOneEnable = VK_FALSE;

			m_cbas.blendEnable = VK_FALSE;
			m_cbas.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //VK_BLEND_FACTOR_SRC_ALPHA
			m_cbas.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
			m_cbas.colorBlendOp = VK_BLEND_OP_ADD;
			m_cbas.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			m_cbas.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			m_cbas.alphaBlendOp = VK_BLEND_OP_ADD;
			m_cbas.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			m_pcbsci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			m_pcbsci.pNext = nullptr;
			m_pcbsci.flags = 0;
			m_pcbsci.logicOpEnable = VK_FALSE;
			m_pcbsci.logicOp = VK_LOGIC_OP_COPY;
			m_pcbsci.attachmentCount = 1;
			m_pcbsci.pAttachments = &m_cbas;
			m_pcbsci.blendConstants[0] = 0.f;
			m_pcbsci.blendConstants[1] = 0.f;
			m_pcbsci.blendConstants[2] = 0.f;
			m_pcbsci.blendConstants[3] = 0.f;
			
			m_pdssci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			m_pdssci.pNext = nullptr;
			m_pdssci.flags = 0;
			m_pdssci.depthTestEnable = VK_FALSE;
			m_pdssci.depthWriteEnable = VK_FALSE;
			m_pdssci.depthCompareOp = VK_COMPARE_OP_LESS;
			m_pdssci.depthBoundsTestEnable = VK_FALSE;
			m_pdssci.stencilTestEnable = VK_FALSE;
			m_pdssci.front = {};
			m_pdssci.back = {};
			m_pdssci.minDepthBounds = 0.0f;
			m_pdssci.maxDepthBounds = 1.0f;

			m_init = true;
		}

		void Pipeline::create(const Device* pDevice, const RenderPass* pRenderPass, const VertexShader* pVertexShader, const FragmentShader* pFragmentShader)
		{
			if (m_created)throw AlreadyCreatedException();
			if (!m_init)throw NotInitializedException();

			m_pDevice = pDevice;;
			
			VkPipelineViewportStateCreateInfo pvsci;
			pvsci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			pvsci.pNext = nullptr;
			pvsci.flags = 0;
			pvsci.viewportCount = m_viewports.size();
			pvsci.pViewports = m_viewports.raw();
			pvsci.scissorCount = m_scissors.size();
			pvsci.pScissors = m_scissors.raw();

			VkPipelineDynamicStateCreateInfo pdsci;
			pdsci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			pdsci.pNext = nullptr;
			pdsci.flags = 0;
			pdsci.dynamicStateCount = m_dynamicStates.size();
			pdsci.pDynamicStates = m_dynamicStates.raw();

			VkPipelineVertexInputStateCreateInfo pvisci;
			pvisci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pvisci.pNext = nullptr;
			pvisci.flags = 0;
			pvisci.vertexBindingDescriptionCount = m_vertexBindingDescriptions.size();
			pvisci.pVertexBindingDescriptions = m_vertexBindingDescriptions.raw();
			pvisci.vertexAttributeDescriptionCount = m_vertexAttributeDescriptions.size();
			pvisci.pVertexAttributeDescriptions = m_vertexAttributeDescriptions.raw();

			lw::List<VkPipelineShaderStageCreateInfo> shaderStages =
			{
				pVertexShader->getStageInfo(), pFragmentShader->getStageInfo()
			};

			VkPipelineLayoutCreateInfo plci;
			plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			plci.pNext = nullptr;
			plci.flags = 0;
			plci.setLayoutCount = m_descriptorSetLayouts.size();
			plci.pSetLayouts = m_descriptorSetLayouts.raw();
			plci.pushConstantRangeCount = m_pushConstantRanges.size();
			plci.pPushConstantRanges = m_pushConstantRanges.raw();

			if (vkCreatePipelineLayout(m_pDevice->raw(), &plci, nullptr, &m_layout) != VK_SUCCESS)
			{
				throw VulkanException("failed to create pipeline layout");
			}

			VkGraphicsPipelineCreateInfo gpci;
			gpci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			gpci.pNext = nullptr;
			gpci.flags = 0;
			gpci.stageCount = shaderStages.size();
			gpci.pStages = shaderStages.raw();
			gpci.pVertexInputState = &pvisci;
			gpci.pInputAssemblyState = &m_piasci;
			gpci.pTessellationState = nullptr;
			gpci.pViewportState = &pvsci;
			gpci.pRasterizationState = &m_prsci;
			gpci.pMultisampleState = &m_pmsci;
			gpci.pDepthStencilState = &m_pdssci;
			gpci.pColorBlendState = &m_pcbsci;
			gpci.pDynamicState = m_dynamicStates.size() == 0 ? nullptr : &pdsci;
			gpci.layout = m_layout;
			gpci.renderPass = pRenderPass->raw();
			gpci.subpass = 0;
			gpci.basePipelineHandle = VK_NULL_HANDLE;
			gpci.basePipelineIndex = -1;

			if (vkCreateGraphicsPipelines(m_pDevice->raw(), VK_NULL_HANDLE, 1, &gpci, nullptr, &m_pipeline) != VK_SUCCESS)
			{
				throw VulkanException("failed to create graphics pipeline");
			}
		}

		void Pipeline::destroy()
		{
			if (!m_init)return;

			vkDestroyPipeline(m_pDevice->raw(), m_pipeline, nullptr);
			m_pipeline = VK_NULL_HANDLE;
			vkDestroyPipelineLayout(m_pDevice->raw(), m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;

			
			m_descriptorSetLayouts.clear();
			m_vertexAttributeDescriptions.clear();
			m_vertexBindingDescriptions.clear();
			m_pushConstantRanges.clear();
			m_pdssci = {};
			m_dynamicStates.clear();
			m_pcbsci = {};
			m_cbas = {};
			m_pmsci = {};
			m_prsci = {};
			m_scissors.clear();
			m_viewports.clear();
			m_piasci = {};

			m_created = false;
			m_init = false;
		}

		VkPipeline Pipeline::raw() const
		{
			if (m_pipeline == VK_NULL_HANDLE)throw NotCreatedException();
			return m_pipeline;
		}

		VkPipelineLayout Pipeline::getLayout() const
		{
			if (m_pipeline == VK_NULL_HANDLE)throw NotCreatedException();
			return m_layout;
		}

		void Pipeline::bind(const CommandBuffer& cmd, VkPipelineBindPoint bindPoint) const
		{
			vkCmdBindPipeline(cmd.raw(), bindPoint, m_pipeline);
		}

		Pipeline& Pipeline::setTopology(VkPrimitiveTopology topology)
		{
			checkInitNotCreated();
			m_piasci.topology = topology;
			return *this;
		}

		Pipeline& Pipeline::setPrimitiveRestartEnable(bool enable)
		{
			checkInitNotCreated();
			m_piasci.primitiveRestartEnable = enable ? VK_TRUE : VK_FALSE;
			return *this;
		}

		Pipeline& Pipeline::addViewport(const VkViewport& viewport, const VkRect2D& scissor)
		{
			checkInitNotCreated();
			m_viewports.clear();
			m_scissors.clear();
			m_viewports.push(viewport);
			m_scissors.push(scissor);
			return *this;
		}

		Pipeline& Pipeline::setDepthClambEnable(bool enable)
		{
			checkInitNotCreated();
			m_prsci.depthClampEnable = enable ? VK_TRUE : VK_FALSE;
			return *this;
		}

		Pipeline& Pipeline::setRasterizerDiscardEnable(bool enable)
		{
			checkInitNotCreated();
			m_prsci.rasterizerDiscardEnable = enable ? VK_TRUE : VK_FALSE;
			return *this;
		}

		Pipeline& Pipeline::setPolygonMode(VkPolygonMode mode)
		{
			checkInitNotCreated();
			m_prsci.polygonMode = mode;
			return *this;
		}

		Pipeline& Pipeline::setCulling(VkCullModeFlags mode, VkFrontFace frontFace)
		{
			checkInitNotCreated();
			m_prsci.cullMode = mode;
			m_prsci.frontFace = frontFace;
			return *this;
		}

		Pipeline& Pipeline::setDepthBias(bool enable, f32 constFactor, f32 clamp, f32 slopeFactor)
		{
			checkInitNotCreated();
			m_prsci.depthBiasEnable = enable ? VK_TRUE : VK_FALSE;
			m_prsci.depthBiasConstantFactor = constFactor;
			m_prsci.depthBiasClamp = clamp;
			m_prsci.depthBiasSlopeFactor = slopeFactor;
			return *this;
		}

		Pipeline& Pipeline::setLineWidth(f32 width)
		{
			checkInitNotCreated();
			m_prsci.lineWidth = width;
			return *this;
		}

		Pipeline& Pipeline::addDynamicState(VkDynamicState state)
		{
			checkInitNotCreated();
			m_dynamicStates.push(state);
			return *this;
		}

		Pipeline& Pipeline::setColorBlending(const VkPipelineColorBlendAttachmentState& attachment, bool enableLogicOp, VkLogicOp logicOp, f32 blendConst0, f32 blendConst1, f32 blendConst2, f32 blendConst3)
		{
			checkInitNotCreated();
			m_cbas = attachment;
			m_pcbsci.logicOpEnable = enableLogicOp;
			m_pcbsci.logicOp = logicOp;
			m_pcbsci.attachmentCount = 1;
			m_pcbsci.pAttachments = &m_cbas;
			m_pcbsci.blendConstants[0] = blendConst0;
			m_pcbsci.blendConstants[1] = blendConst1;
			m_pcbsci.blendConstants[2] = blendConst2;
			m_pcbsci.blendConstants[3] = blendConst3;
			return *this;
		}

		Pipeline& Pipeline::addVertexBinding(const VkVertexInputBindingDescription& vibd)
		{
			checkInitNotCreated();
			m_vertexBindingDescriptions.push(vibd);
			return *this;
		}

		Pipeline& Pipeline::addVertexBinding(u32 binding, u32 stride, VkVertexInputRate inputRate)
		{
			VkVertexInputBindingDescription vibd;
			vibd.binding = binding;
			vibd.stride = stride;
			vibd.inputRate = inputRate;
			return addVertexBinding(vibd);
		}

		Pipeline& Pipeline::addVertexDescription(const VkVertexInputAttributeDescription& viad)
		{
			checkInitNotCreated();
			m_vertexAttributeDescriptions.push(viad);
			return *this;
		}

		Pipeline& Pipeline::addVertexDescription(u32 location, u32 binding, VkFormat format, u32 offset)
		{
			VkVertexInputAttributeDescription viad;
			viad.location = location;
			viad.binding = binding;
			viad.format = format;
			viad.offset = offset;
			return addVertexDescription(viad);
		}


		Pipeline& Pipeline::setDepthStencil(bool enableDepthTest, bool enableDepthWrite, VkCompareOp depthCompareOp, bool enableDepthBoundsTest, bool enableStencilTest, VkStencilOpState front, VkStencilOpState back, f32 minDepthBounds, f32 maxDepthBounds)
		{
			checkInitNotCreated();
			m_pdssci.depthTestEnable = enableDepthTest ? VK_TRUE : VK_FALSE;
			m_pdssci.depthWriteEnable = enableDepthWrite ? VK_TRUE : VK_FALSE;
			m_pdssci.depthCompareOp = depthCompareOp;
			m_pdssci.depthBoundsTestEnable = enableDepthBoundsTest ? VK_TRUE : VK_FALSE;
			m_pdssci.stencilTestEnable = enableStencilTest ? VK_TRUE : VK_FALSE;
			m_pdssci.front = front;
			m_pdssci.back = back;
			m_pdssci.minDepthBounds = minDepthBounds;
			m_pdssci.maxDepthBounds = maxDepthBounds;
			return *this;
		}

		Pipeline& Pipeline::addDesciptorSetLayout(const VkDescriptorSetLayout& dsl)
		{
			checkInitNotCreated();
			m_descriptorSetLayouts.push(dsl);
			return *this;
		}

		Pipeline& Pipeline::addPushConstantRange(const VkPushConstantRange& pcr)
		{
			checkInitNotCreated();
			m_pushConstantRanges.push(pcr);
			return *this;
		}

		Pipeline& Pipeline::addPushConstantRange(VkShaderStageFlags stageFlags, u32 offset, u32 size)
		{
			VkPushConstantRange pcr;
			pcr.stageFlags = stageFlags;
			pcr.offset = offset;
			pcr.size = size;
			return addPushConstantRange(pcr);
		}

		void Pipeline::checkInitNotCreated()
		{
			if (!m_init)throw NotInitializedException("Pipeline must be initialized, to set parameters.");
			if (m_created)throw AlreadyCreatedException("Pipeline mustn't be created, to set parameters.");
		}

	}
}
