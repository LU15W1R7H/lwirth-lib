#include "../stdafx.hpp"
#include "Renderer.hpp"

#include "Color.hpp"

#include <iostream>
#include "../memory/List.hpp"

namespace lw
{
	void Renderer::start(const Frame* pFrame)
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

	void Renderer::destroy()
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



	SimpleBrush2D* Renderer::getSimpleBrush2D()
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

	lw::SimpleBrush3D* Renderer::getSimpleBrush3D()
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

	void Renderer::preDraw()
	{
		m_imageIndex = m_swapchain.acquireNextImage(m_semaphoreImageAvailable);

		m_drawingCommandBuffer.allocate(&m_mainDevice, &m_commandPool);
		m_drawingCommandBuffer.beginSingleTime();

		lw::List<VkClearValue> clearValues = 
		{
			{ 0.f, 0.f, 0.f, 1.f }, //Color
			{ 1.f, 0 } //Depth
		};

		m_renderPass.begin(m_drawingCommandBuffer, m_swapchain.getFramebuffer(m_imageIndex), { {0, 0}, {m_screenWidth, m_screenHeight } }, clearValues, VK_SUBPASS_CONTENTS_INLINE);

		if(m_pSimpleBrush2D)m_pSimpleBrush2D->prepare(&m_drawingCommandBuffer);
	}

	void Renderer::postDraw()
	{
		if (m_pSimpleBrush2D)m_pSimpleBrush2D->disperse();

		m_renderPass.end(m_drawingCommandBuffer);

		m_drawingCommandBuffer.end();

		m_drawingCommandBuffer.submit(m_mainDevice.getGraphicsQueue(), VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			&m_semaphoreImageAvailable, &m_semaphoreRenderingDone, &m_fence);


		m_swapchain.present(*m_mainDevice.getPresentQueue(), m_semaphoreRenderingDone, m_imageIndex);

		m_fence.wait();
		m_drawingCommandBuffer.free();
		destroyPendingBuffers();
	}


	void Renderer::resize(u32 width, u32 height)
	{
		if (width == 0 || height == 0) return;

		VkExtent2D maxExtent = m_mainDevice.getPhysical()->surfaceCapabilities().maxImageExtent;
		if (width > maxExtent.width) width = maxExtent.width;
		if (height > maxExtent.height) height = maxExtent.height;

		m_screenWidth = width;
		m_screenHeight = height;

		recreateSwapchain();
	}

	void Renderer::submitBufferToDestroy(const VK::Buffer& buffer)
	{
		m_bufferDestroyQueue.push(buffer);
	}

	void Renderer::recreateSwapchain()
	{
		m_mainDevice.waitIdle();

		m_renderPass.destroy();
		m_depthImage.destroy();


		VK::Swapchain newChain;
		m_renderPass.create(&m_mainDevice);
		m_depthImage.create(&m_mainDevice, &m_commandPool, m_screenWidth, m_screenHeight);
		newChain.create(&m_mainDevice, &m_surface, &m_depthImage, &m_renderPass, m_screenWidth, m_screenHeight, &m_swapchain);
		if(m_pSimpleBrush2D)m_pSimpleBrush2D->resize(m_screenWidth, m_screenHeight);


		m_swapchain.destroy();
		m_swapchain = newChain;
	}

	void Renderer::destroyPendingBuffers()
	{
		for (size_t i = 0; i < m_bufferDestroyQueue.size(); i++)
		{
			m_bufferDestroyQueue[i].destroy();
		}
		m_bufferDestroyQueue.clear();
	}

}