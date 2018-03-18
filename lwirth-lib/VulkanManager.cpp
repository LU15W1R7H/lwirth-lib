#include "stdafx.hpp"
#include "VulkanManager.hpp"

#include "Matrix4x4.hpp"
#include "Vector2.hpp"
#include "Color.hpp"

#include <iostream>
#include <vector>

namespace lw
{
	namespace VK
	{
		

		void VulkanManager::start(const Frame* pFrame)
		{
			m_instance.create(LWIRTH_NAME, LWIRTH_VERSION_MAJOR, LWIRTH_VERSION_MINOR, LWIRTH_VERSION_PATCH);
			m_pFrame = pFrame;

			m_screenWidth = m_pFrame->getWidth();
			m_screenHeight = m_pFrame->getHeight();

			m_surface.create(&m_instance, m_pFrame);
			m_physicalDeviceContatiner.init(&m_instance, &m_surface);
			m_device.create(&m_physicalDeviceContatiner, &m_surface);
			m_commandPool.create(&m_device);
			m_renderPass.create(&m_device);
			m_depthImage.create(&m_device, &m_commandPool, m_screenWidth, m_screenHeight);
			m_swapchain.create(&m_device, &m_surface, &m_depthImage, &m_renderPass, m_screenWidth, m_screenHeight);
			m_semaphoreImageAvailable.create(&m_device);
			m_semaphoreRenderingDone.create(&m_device);
			m_fence.create(&m_device);

			m_simpleBrush2D.create(&m_device, &m_renderPass, &m_commandPool, m_screenWidth, m_screenHeight);
		}

		void VulkanManager::destroy()
		{
			m_device.waitIdle();
			//destroy rest

			m_simpleBrush2D.destroy();

			m_descriptorPool.destroy();

			m_fence.destroy();
			m_semaphoreRenderingDone.destroy();
			m_semaphoreImageAvailable.destroy();
			m_swapchain.destroy();
			m_depthImage.destroy();
			m_commandPool.destroy();
			m_renderPass.destroy();
			m_device.destroy();
			m_surface.destroy();
			m_instance.destroy();
		}



		SimpleBrush2D* VulkanManager::preDraw()
		{
			VkResult result = vkAcquireNextImageKHR(m_device.raw(), m_swapchain.raw(), std::numeric_limits<uint64_t>::max(), m_semaphoreImageAvailable.raw(), VK_NULL_HANDLE, &m_imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				throw VulkanException("out of date");
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw VulkanException("failed to acquire swapchain image");
			}

			m_drawingCommandBuffer.allocate(&m_device, &m_commandPool);

			m_drawingCommandBuffer.beginSingleTime();

			VkRenderPassBeginInfo rpbi;
			rpbi.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
			rpbi.pNext = nullptr;
			rpbi.renderPass = m_renderPass.raw();
			rpbi.framebuffer = m_swapchain.getFramebuffer(m_imageIndex);
			rpbi.renderArea.offset = { 0, 0 };
			rpbi.renderArea.extent = { m_screenWidth, m_screenHeight };

			VkClearValue clearValue = { 0.f, 0.f, 0.f, 1.f };
			VkClearValue depthClearValue = { 1.f, 0 };
			std::vector<VkClearValue> clearValues = { clearValue, depthClearValue };

			rpbi.clearValueCount = clearValues.size();
			rpbi.pClearValues = clearValues.data();

			vkCmdBeginRenderPass(m_drawingCommandBuffer.raw(), &rpbi, VK_SUBPASS_CONTENTS_INLINE);

			VkViewport viewport;
			viewport.x = 0.f;
			viewport.y = 0.f;
			viewport.width = m_screenWidth;
			viewport.height = m_screenHeight;
			viewport.minDepth = 0.f;
			viewport.maxDepth = 1.f;
			vkCmdSetViewport(m_drawingCommandBuffer.raw(), 0, 1, &viewport);

			VkRect2D scissor;
			scissor.offset = { 0, 0 };
			scissor.extent = { m_screenWidth, m_screenHeight };
			vkCmdSetScissor(m_drawingCommandBuffer.raw(), 0, 1, &scissor);

			m_simpleBrush2D.prepareDrawing(&m_drawingCommandBuffer);
			return &m_simpleBrush2D;
		}

		void VulkanManager::postDraw()
		{
			vkCmdEndRenderPass(m_drawingCommandBuffer.raw());

			m_drawingCommandBuffer.end();

			m_drawingCommandBuffer.submit(m_device.getGraphicsQueue(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
				&m_semaphoreImageAvailable, &m_semaphoreRenderingDone, &m_fence);


			VkPresentInfoKHR pi;
			pi.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
			pi.pNext = nullptr;
			pi.waitSemaphoreCount = 1;
			pi.pWaitSemaphores = m_semaphoreRenderingDone.ptr();
			pi.swapchainCount = 1;
			pi.pSwapchains = m_swapchain.ptr();
			pi.pImageIndices = &m_imageIndex;
			pi.pResults = nullptr;


			VkResult result = vkQueuePresentKHR(m_device.getPresentQueue()->raw(), &pi);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR) //TODO fix -> result == OUT_OF_DATE
			{
				throw VulkanException("presentation was out of date or suboptimal");
				//recreateSwapchain();
			}
			else if (result != VK_SUCCESS)
			{
				throw VulkanException("failed to present swapchain image");
			}

			m_fence.wait();
			m_drawingCommandBuffer.free();
		}


		void VulkanManager::resize(U32 width, U32 height)
		{
			if (width == 0 || height == 0) return;

			VkExtent2D maxExtent = m_device.getPhysical()->surfaceCapabilities().maxImageExtent;
			if (width > maxExtent.width) width = maxExtent.width;
			if (height > maxExtent.height) height = maxExtent.height;

			m_screenWidth = width;
			m_screenHeight = height;

			recreateSwapchain();
		}

		void VulkanManager::recreateSwapchain()
		{
			m_device.waitIdle();

			m_simpleBrush2D.destroyPipeline();
			m_renderPass.destroy();
			m_depthImage.destroy();


			Swapchain newChain;
			m_renderPass.create(&m_device);
			m_depthImage.create(&m_device, &m_commandPool, m_screenWidth, m_screenHeight);
			newChain.create(&m_device, &m_surface, &m_depthImage, &m_renderPass, m_screenWidth, m_screenHeight, &m_swapchain);
			m_simpleBrush2D.createPipeline(&m_renderPass);

			m_swapchain.destroy();
			m_swapchain = newChain;
		}
	}

}