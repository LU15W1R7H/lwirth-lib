#include "stdafx.hpp"
#include "Vulkan.hpp"

#include "Color.hpp"

#include <iostream>
#include "List.hpp"

namespace lw
{
	namespace VK
	{
		void Vulkan::start(const Frame* pFrame)
		{
			m_instance.create(LW_NAME, LW_VER_MAJOR, LW_VER_MINOR, LW_VER_PATCH, VK_API_VERSION_1_1);
			m_pFrame = pFrame;

			m_screenWidth = m_pFrame->getWidth();
			m_screenHeight = m_pFrame->getHeight();

			m_surface.create(&m_instance, m_pFrame);
			m_physicalDeviceContatiner.init(&m_instance, &m_surface);
			m_mainDevice.create(&m_physicalDeviceContatiner, &m_surface);
			m_commandPool.create(&m_mainDevice);
			m_renderPass.create(&m_mainDevice);
			m_depthImage.create(&m_mainDevice, &m_commandPool, m_screenWidth, m_screenHeight);
			m_swapchain.create(&m_mainDevice, &m_surface, &m_depthImage, &m_renderPass, m_screenWidth, m_screenHeight);
			m_semaphoreImageAvailable.create(&m_mainDevice);
			m_semaphoreRenderingDone.create(&m_mainDevice);
			m_fence.create(&m_mainDevice);
		}

		void Vulkan::destroy()
		{
			m_mainDevice.waitIdle();
			//destroy rest

			if (m_pSimpleBrush2D)m_pSimpleBrush2D->destroy();
			delete m_pSimpleBrush2D;

			m_descriptorPool.destroy();

			m_fence.destroy();
			m_semaphoreRenderingDone.destroy();
			m_semaphoreImageAvailable.destroy();
			m_swapchain.destroy();
			m_depthImage.destroy();
			m_commandPool.destroy();
			m_renderPass.destroy();
			m_mainDevice.destroy();
			m_surface.destroy();
			m_instance.destroy();
		}



		SimpleBrush2D* Vulkan::getSimpleBrush2D()
		{
			if (m_pSimpleBrush2D)
			{
				return m_pSimpleBrush2D;
			}
			else
			{
				m_pSimpleBrush2D = new SimpleBrush2D();
				m_pSimpleBrush2D->create(this, m_screenWidth, m_screenHeight);
				return m_pSimpleBrush2D;
			}
		}

		lw::SimpleBrush3D* Vulkan::getSimpleBrush3D()
		{
			if (m_pSimpleBrush3D)
			{
				return m_pSimpleBrush3D;
			}
			else
			{
				m_pSimpleBrush3D = new SimpleBrush3D();
				m_pSimpleBrush3D->create(this, m_screenWidth, m_screenHeight);
				return m_pSimpleBrush3D;
			}
		}

		void Vulkan::preDraw()
		{
			VkResult result = vkAcquireNextImageKHR(m_mainDevice.raw(), m_swapchain.raw(), std::numeric_limits<uint64_t>::max(), m_semaphoreImageAvailable.raw(), VK_NULL_HANDLE, &m_imageIndex);

			if (result == VK_ERROR_OUT_OF_DATE_KHR)
			{
				throw VulkanException("out of date");
			}
			else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR)
			{
				throw VulkanException("failed to acquire swapchain image");
			}

			m_drawingCommandBuffer.allocate(&m_mainDevice, &m_commandPool);

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
			lw::List<VkClearValue> clearValues = { clearValue, depthClearValue };

			rpbi.clearValueCount = clearValues.size();
			rpbi.pClearValues = clearValues.raw();

			vkCmdBeginRenderPass(m_drawingCommandBuffer.raw(), &rpbi, VK_SUBPASS_CONTENTS_INLINE);


			if(m_pSimpleBrush2D)m_pSimpleBrush2D->prepare(&m_drawingCommandBuffer);
		}

		void Vulkan::postDraw()
		{
			if (m_pSimpleBrush2D)m_pSimpleBrush2D->disperse();

			vkCmdEndRenderPass(m_drawingCommandBuffer.raw());

			m_drawingCommandBuffer.end();

			m_drawingCommandBuffer.submit(m_mainDevice.getGraphicsQueue(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
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


			VkResult result = vkQueuePresentKHR(m_mainDevice.getPresentQueue()->raw(), &pi);

			if (result == VK_ERROR_OUT_OF_DATE_KHR || result == VK_SUBOPTIMAL_KHR)
			{
				throw VulkanException("presentation was out of date or suboptimal");
			}
			else if (result != VK_SUCCESS)
			{
				throw VulkanException("failed to present swapchain image");
			}

			m_fence.wait();
			m_drawingCommandBuffer.free();
			destroyPendingBuffers();
		}


		void Vulkan::resize(u32 width, u32 height)
		{
			if (width == 0 || height == 0) return;

			VkExtent2D maxExtent = m_mainDevice.getPhysical()->surfaceCapabilities().maxImageExtent;
			if (width > maxExtent.width) width = maxExtent.width;
			if (height > maxExtent.height) height = maxExtent.height;

			m_screenWidth = width;
			m_screenHeight = height;

			recreateSwapchain();
		}

		void Vulkan::submitBufferToDestroy(const Buffer& buffer)
		{
			m_bufferDestroyQueue.push(buffer);
		}

		void Vulkan::recreateSwapchain()
		{
			m_mainDevice.waitIdle();

			m_renderPass.destroy();
			m_depthImage.destroy();


			Swapchain newChain;
			m_renderPass.create(&m_mainDevice);
			m_depthImage.create(&m_mainDevice, &m_commandPool, m_screenWidth, m_screenHeight);
			newChain.create(&m_mainDevice, &m_surface, &m_depthImage, &m_renderPass, m_screenWidth, m_screenHeight, &m_swapchain);
			if(m_pSimpleBrush2D)m_pSimpleBrush2D->resize(m_screenWidth, m_screenHeight);


			m_swapchain.destroy();
			m_swapchain = newChain;
		}

		void Vulkan::destroyPendingBuffers()
		{
			for (size_t i = 0; i < m_bufferDestroyQueue.size(); i++)
			{
				m_bufferDestroyQueue[i].destroy();
			}
			m_bufferDestroyQueue.clear();
		}

	}

}