#include "stdafx.hpp"
#include "VulkanPipeline.hpp"


#include "Exceptions.hpp"
#include "VulkanShader.hpp"
#include "VulkanDevice.hpp"
#include "VulkanRenderPass.hpp"
#include "FileManager.hpp"

namespace lw
{
	namespace VK
	{
		Pipeline::Pipeline()
		{
		}

		Pipeline::~Pipeline()
		{
			if (m_pipeline != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void Pipeline::create(const Device* pDevice, const RenderPass* pRenderPass, const VertexShader* pVertexShader, const FragmentShader* pFragmentShader, U32 screenWidth, U32 screenHeight)
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;;
			
			VkPipelineInputAssemblyStateCreateInfo piasci;
			piasci.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
			piasci.pNext = nullptr;
			piasci.flags = 0;
			piasci.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
			piasci.primitiveRestartEnable = VK_FALSE;

			VkViewport viewport;
			viewport.x = 0.f;
			viewport.y = 0.f;
			viewport.width = static_cast<float>(screenWidth);
			viewport.height = static_cast<float>(screenHeight);
			viewport.minDepth = 0.f;
			viewport.maxDepth = 1.f;

			VkRect2D scissor;
			scissor.offset = { 0, 0 };
			scissor.extent = { screenWidth, screenHeight };

			VkPipelineViewportStateCreateInfo pvsci;
			pvsci.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
			pvsci.pNext = nullptr;
			pvsci.flags = 0;
			pvsci.viewportCount = 1;
			pvsci.pViewports = &viewport;
			pvsci.scissorCount = 1;
			pvsci.pScissors = &scissor;

			VkPipelineRasterizationStateCreateInfo prsci;
			prsci.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
			prsci.pNext = nullptr;
			prsci.flags = 0;
			prsci.depthClampEnable = VK_FALSE;
			prsci.rasterizerDiscardEnable = VK_FALSE;
			prsci.polygonMode = VK_POLYGON_MODE_FILL;
			prsci.cullMode = VK_CULL_MODE_NONE; //SHOULD BE BACK_BIT
			prsci.frontFace = VK_FRONT_FACE_CLOCKWISE;
			prsci.depthBiasEnable = VK_FALSE;
			prsci.depthBiasConstantFactor = 0.f;
			prsci.depthBiasClamp = 0.f;
			prsci.depthBiasSlopeFactor = 0.f;
			prsci.lineWidth = 1.0f;

			VkPipelineMultisampleStateCreateInfo pmsci;
			pmsci.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
			pmsci.pNext = nullptr;
			pmsci.flags = 0;
			pmsci.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
			pmsci.sampleShadingEnable = VK_FALSE;
			pmsci.minSampleShading = 1.f;
			pmsci.pSampleMask = nullptr;
			pmsci.alphaToCoverageEnable = VK_FALSE;
			pmsci.alphaToOneEnable = VK_FALSE;
			
			VkPipelineColorBlendAttachmentState cbas;
			cbas.blendEnable = VK_FALSE;
			cbas.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; //VK_BLEND_FACTOR_SRC_ALPHA
			cbas.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; //VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA
			cbas.colorBlendOp = VK_BLEND_OP_ADD;
			cbas.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
			cbas.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
			cbas.alphaBlendOp = VK_BLEND_OP_ADD;
			cbas.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;

			VkPipelineColorBlendStateCreateInfo pcbsci;
			pcbsci.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
			pcbsci.pNext = nullptr;
			pcbsci.flags = 0;
			pcbsci.logicOpEnable = VK_FALSE;
			pcbsci.logicOp = VK_LOGIC_OP_COPY;
			pcbsci.attachmentCount = 1;
			pcbsci.pAttachments = &cbas;
			pcbsci.blendConstants[0] = 0.f;
			pcbsci.blendConstants[1] = 0.f;
			pcbsci.blendConstants[2] = 0.f;
			pcbsci.blendConstants[3] = 0.f;

			std::vector<VkDynamicState> dynamicStates =
			{
				VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR
			};

			VkPipelineDynamicStateCreateInfo pdsci;
			pdsci.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
			pdsci.pNext = nullptr;
			pdsci.flags = 0;
			pdsci.dynamicStateCount = dynamicStates.size();
			pdsci.pDynamicStates = dynamicStates.data();

			//CREATION down here

			VkPipelineVertexInputStateCreateInfo pvisci;
			pvisci.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
			pvisci.pNext = nullptr;
			pvisci.flags = 0;
			pvisci.vertexBindingDescriptionCount = m_vertexBindingDescriptions.size();
			pvisci.pVertexBindingDescriptions = m_vertexBindingDescriptions.data();
			pvisci.vertexAttributeDescriptionCount = m_vertexAttributeDescriptions.size();
			pvisci.pVertexAttributeDescriptions = m_vertexAttributeDescriptions.data();

			VkPipelineDepthStencilStateCreateInfo pdssci;
			pdssci.sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO;
			pdssci.pNext = nullptr;
			pdssci.flags = 0;
			pdssci.depthTestEnable = VK_TRUE;
			pdssci.depthWriteEnable = VK_TRUE;
			pdssci.depthCompareOp = VK_COMPARE_OP_LESS;
			pdssci.depthBoundsTestEnable = VK_FALSE;
			pdssci.stencilTestEnable = VK_FALSE;
			pdssci.front = {};
			pdssci.back = {};
			pdssci.minDepthBounds = 0.0f;
			pdssci.maxDepthBounds = 1.0f;

			std::vector<VkPipelineShaderStageCreateInfo> shaderStages =
			{
				pVertexShader->getStageInfo(), pFragmentShader->getStageInfo()
			};

			VkPipelineLayoutCreateInfo plci;
			plci.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
			plci.pNext = nullptr;
			plci.flags = 0;
			plci.setLayoutCount = m_descriptorSetLayouts.size();
			plci.pSetLayouts = m_descriptorSetLayouts.data();
			plci.pushConstantRangeCount = m_pushConstantRanges.size();
			plci.pPushConstantRanges = m_pushConstantRanges.data();

			if (vkCreatePipelineLayout(m_pDevice->raw(), &plci, nullptr, &m_layout) != VK_SUCCESS)
			{
				throw VulkanException("failed to create pipeline layout");
			}

			VkGraphicsPipelineCreateInfo gpci;
			gpci.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
			gpci.pNext = nullptr;
			gpci.flags = 0;
			gpci.stageCount = shaderStages.size();
			gpci.pStages = shaderStages.data();
			gpci.pVertexInputState = &pvisci;
			gpci.pInputAssemblyState = &piasci;
			gpci.pTessellationState = nullptr;
			gpci.pViewportState = &pvsci;
			gpci.pRasterizationState = &prsci;
			gpci.pMultisampleState = &pmsci;
			gpci.pDepthStencilState = &pdssci;
			gpci.pColorBlendState = &pcbsci;
			gpci.pDynamicState = &pdsci;
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
			if (m_pipeline == VK_NULL_HANDLE)return;

			vkDestroyPipeline(m_pDevice->raw(), m_pipeline, nullptr);
			m_pipeline = VK_NULL_HANDLE;
			vkDestroyPipelineLayout(m_pDevice->raw(), m_layout, nullptr);
			m_layout = VK_NULL_HANDLE;

			
			m_pushConstantRanges.clear();
			m_vertexBindingDescriptions.clear();
			m_vertexAttributeDescriptions.clear();
			m_descriptorSetLayouts.clear();
			
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

		void Pipeline::addVertexBinding(const VkVertexInputBindingDescription * vibd)
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();
			m_vertexBindingDescriptions.push_back(*vibd);
		}

		void Pipeline::addVertexBinding(U32 binding, U32 stride, VkVertexInputRate inputRate)
		{
			VkVertexInputBindingDescription vibd;
			vibd.binding = binding;
			vibd.stride = stride;
			vibd.inputRate = inputRate;
			addVertexBinding(&vibd);
		}

		void Pipeline::addVertexDescription(const VkVertexInputAttributeDescription * viad)
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();
			m_vertexAttributeDescriptions.push_back(*viad);
		}

		void Pipeline::addVertexDescription(U32 location, U32 binding, VkFormat format, U32 offset)
		{
			VkVertexInputAttributeDescription viad;
			viad.location = location;
			viad.binding = binding;
			viad.format = format;
			viad.offset = offset;
			addVertexDescription(&viad);
		}

		void Pipeline::addDesciptorSetLayout(const VkDescriptorSetLayout * dsl)
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();
			m_descriptorSetLayouts.push_back(*dsl);
		}

		void Pipeline::addPushConstantRange(const VkPushConstantRange * pcr)
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();
			m_pushConstantRanges.push_back(*pcr);
		}

		void Pipeline::addPushConstantRange(VkShaderStageFlags stageFlags, U32 offset, U32 size)
		{
			VkPushConstantRange pcr;
			pcr.stageFlags = stageFlags;
			pcr.offset = offset;
			pcr.size = size;
			addPushConstantRange(&pcr);
		}

		void Pipeline::enableDepthBuffer()
		{
			if (m_pipeline != VK_NULL_HANDLE)throw AlreadyCreatedException();
			m_useDepthBuffer = true;
		}
	}
}
