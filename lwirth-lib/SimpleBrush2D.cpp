#include "stdafx.hpp"
#include "SimpleBrush2D.hpp"

#include "Vector2.hpp"
#include "Color.hpp"

#include "Vertices.hpp"
#include "VulkanDevice.hpp"


namespace lw
{

	

	void SimpleBrush2D::fillTriangle(const Triangle* tri)
	{
		float pushConstantsPos[6] = { tri->m_vertices[0].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[0].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[1].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[1].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[2].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[2].pos.y / m_screenHeight * 2.f - 1.f };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_pipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 6, pushConstantsPos);

		float pushConstantsColor[4] = { tri->m_vertices[0].color.r, tri->m_vertices[0].color.g, tri->m_vertices[0].color.b, tri->m_vertices[0].color.a };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_pipeline.getLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(float) * 4, pushConstantsColor);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, Triangle::s_vertexBuffer.ptr(), offsets);
		vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), Triangle::s_indexBuffer.raw(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(m_pCmdBuffer->raw(), 3, 1, 0, 0, 0);
	}

	void SimpleBrush2D::fillVertexArray(VertexArray& vertexArray)
	{
		vertexArray.updateBuffer(m_pDevice, m_pCommandPool);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, vertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), vertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush2D::create(const VK::Device* pDevice, const VK::RenderPass* pRenderPass, const VK::CommandPool* pCommandPool, U32 screenWidth, U32 screenHeight)
	{
		m_pDevice = pDevice;
		m_pCommandPool = pCommandPool;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_vertexShader.create(m_pDevice, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshadervert.spv");
		m_fragmentShader.create(m_pDevice, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshaderfrag.spv");


		SimpleBrush2D::createPipeline(pRenderPass);

		Triangle::s_init(m_pDevice, m_pCommandPool);
	}

	void SimpleBrush2D::destroy()
	{
		Triangle::s_deinit();

		SimpleBrush2D::destroyPipeline();

		m_fragmentShader.destroy();
		m_vertexShader.destroy();
	}

	void SimpleBrush2D::createPipeline(const VK::RenderPass* pRenderPass)
	{
		m_pipeline.init(m_screenWidth, m_screenHeight);
		m_pipeline.addVertexBinding(0, sizeof(Vertex), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pipeline.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex, Vertex::pos));
		m_pipeline.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex, Vertex::color));
		m_pipeline.create(m_pDevice, pRenderPass, &m_vertexShader, &m_fragmentShader);
	}

	void SimpleBrush2D::destroyPipeline()
	{
		m_pipeline.destroy();
	}

	void SimpleBrush2D::prepareDrawing(const VK::CommandBuffer* cmdBuffer)
	{
		m_pCmdBuffer = cmdBuffer;
		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipeline.raw());
	}
}