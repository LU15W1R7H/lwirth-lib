#include "../../stdafx.hpp"
#include "VulkanRenderPass.hpp"

#include "../../general/Exceptions.hpp"
#include "VulkanDevice.hpp"
#include "VulkanHelper.hpp"

#include "VulkanDepthImage.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	namespace VK
	{

		RenderPass::~RenderPass()
		{
			if (m_renderPass != VK_NULL_HANDLE)throw NotDestroyedException();
		}

		void RenderPass::create(Device* pDevice)
		{
			if (m_renderPass != VK_NULL_HANDLE)throw AlreadyCreatedException();

			m_pDevice = pDevice;

			VkAttachmentDescription colorAttachment;
			colorAttachment.flags = 0;
			colorAttachment.format = m_pDevice->getSurfaceFormat().format;
			colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
			colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
			colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
			colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
			colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
			colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
			colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

			VkAttachmentReference colorAttachmentRef;
			colorAttachmentRef.attachment = 0;
			colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

			VkAttachmentDescription depthAttachment = DepthImage::getDepthAttachment(pDevice->getPhysical());

			VkAttachmentReference depthAttachmentRef;
			depthAttachmentRef.attachment = 1;
			depthAttachmentRef.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;

			VkSubpassDescription subpass;
			subpass.flags = 0;
			subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
			subpass.inputAttachmentCount = 0;
			subpass.pInputAttachments = nullptr;
			subpass.colorAttachmentCount = 1;
			subpass.pColorAttachments = &colorAttachmentRef;
			subpass.pResolveAttachments = nullptr;
			subpass.pDepthStencilAttachment = &depthAttachmentRef;
			subpass.preserveAttachmentCount = 0;
			subpass.pPreserveAttachments = nullptr;

			VkSubpassDependency dependency;
			dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
			dependency.dstSubpass = 0;
			dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
			dependency.srcAccessMask = 0;
			dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
			dependency.dependencyFlags = 0;

			lw::List<VkAttachmentDescription> attachments = { colorAttachment, depthAttachment };
			
			VkRenderPassCreateInfo rpci;
			rpci.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
			rpci.pNext = nullptr;
			rpci.flags = 0;
			rpci.attachmentCount = attachments.size();
			rpci.pAttachments = attachments.raw();
			rpci.subpassCount = 1;
			rpci.pSubpasses = &subpass;
			rpci.dependencyCount = 1;
			rpci.pDependencies = &dependency;

			if (vkCreateRenderPass(m_pDevice->raw(), &rpci, nullptr, &m_renderPass) != VK_SUCCESS)
			{
				throw VulkanException("failed to create render pass");
			}
		}

		void RenderPass::destroy()
		{
			if (m_renderPass == VK_NULL_HANDLE)return;

			vkDestroyRenderPass(m_pDevice->raw(), m_renderPass, nullptr);
			m_renderPass = VK_NULL_HANDLE;
			m_pDevice = nullptr;
		}

		void RenderPass::begin(const CommandBuffer & cmd, VkFramebuffer framebuffer, VkRect2D renderArea, const List<VkClearValue>& clearValues, VkSubpassContents contents)
		{
			VkRenderPassBeginInfo rpbi;
			rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			rpbi.pNext = nullptr;
			rpbi.renderPass = m_renderPass;
			rpbi.framebuffer = framebuffer;
			rpbi.renderArea = renderArea;
			rpbi.clearValueCount = clearValues.size();
			rpbi.pClearValues = clearValues.raw();

			vkCmdBeginRenderPass(cmd.raw(), &rpbi, contents);
		}

		VkRenderPass RenderPass::raw() const
		{
			if (m_renderPass == VK_NULL_HANDLE)throw NotCreatedException();
			return m_renderPass;
		}

		VkRenderPass* RenderPass::ptr()
		{
			if (m_renderPass == VK_NULL_HANDLE)throw NotCreatedException();
			return &m_renderPass;
		}
	}
}