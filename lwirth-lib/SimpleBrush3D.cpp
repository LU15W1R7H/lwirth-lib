#include "stdafx.hpp"

#include "SimpleBrush3D.hpp"
#include "Color.hpp"

#include "Vulkan.hpp"


namespace lw
{


	void SimpleBrush3D::fillTriangle(const Triangle* tri)
	{
		float pushConstantsPos[6] = { tri->m_vertices[0].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[0].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[1].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[1].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[2].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[2].pos.y / m_screenHeight * 2.f - 1.f };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_pipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 6, pushConstantsPos);

		float pushConstantsColor[4] = { tri->m_vertices[0].color.r, tri->m_vertices[0].color.g, tri->m_vertices[0].color.b, tri->m_vertices[0].color.a };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_pipeline.getLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(float) * 4, pushConstantsColor);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, Triangle::s_vertexBuffer.ptr(), offsets);
		//vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), Triangle::s_indexBuffer.raw(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(m_pCmdBuffer->raw(), 3, 1, 0, 0, 0);
	}

	void SimpleBrush3D::fillVertexArray(Vertex3DArray& vertexArray)
	{
		//vertexArray.updateBuffer(m_pDevice, m_pCommandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, vertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), vertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush3D::create(VK::Vulkan* pVulkan, U32 screenWidth, U32 screenHeight)
	{
		m_pVK = pVulkan;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_vertexShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/vert.spv");
		m_fragmentShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/frag.spv");

		SimpleBrush3D::createPipeline();
	}

	void SimpleBrush3D::destroy()
	{
		SimpleBrush3D::destroyPipeline();

		m_fragmentShader.destroy();
		m_vertexShader.destroy();
	}

	void SimpleBrush3D::createPipeline()
	{
		m_pipeline.init(m_screenWidth, m_screenHeight);
		m_pipeline.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_pipeline.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_pipeline.addVertexBinding(0, sizeof(Vertex3D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pipeline.addVertexDescription(0, 0, VK_FORMAT_R32G32B32_SFLOAT, offsetof(Vertex3D, Vertex3D::pos));
		m_pipeline.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex3D, Vertex3D::color));
		m_pipeline.setDepthStencil(true, true, VK_COMPARE_OP_LESS, false, false, {}, {}, 0.f, 1.f);
		m_pipeline.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);
	}

	void SimpleBrush3D::destroyPipeline()
	{
		m_pipeline.destroy();
	}

	void SimpleBrush3D::prepare(const VK::CommandBuffer* cmd)
	{
		m_pCmdBuffer = cmd;
		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.raw());

		VkViewport viewport;
		viewport.x = 0.f;
		viewport.y = 0.f;
		viewport.width = static_cast<F32>(m_screenWidth);
		viewport.height = static_cast<F32>(m_screenHeight);
		viewport.minDepth = 0.f;
		viewport.maxDepth = 1.f;

		VkRect2D scissor;
		scissor.offset = { 0, 0 };
		scissor.extent = { m_screenWidth, m_screenHeight };

		vkCmdSetViewport(cmd->raw(), 0, 1, &viewport);
		vkCmdSetScissor(cmd->raw(), 0, 1, &scissor);

		m_ready = true;
	}

	void SimpleBrush3D::disperse()
	{
		m_pCmdBuffer = nullptr;

		m_ready = false;
	}

	void SimpleBrush3D::resize(U32 screenWidth, U32 screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		destroyPipeline();
		createPipeline();
	}

}