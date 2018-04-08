#include "stdafx.hpp"
#include "SimpleBrush2D.hpp"

#include "Color.hpp"

#include "Vulkan.hpp"


namespace lw
{

	

	

	void SimpleBrush2D::setColor(const Color& color)
	{
		m_mainColor = color;
	}

	void SimpleBrush2D::setColor(F32 r, F32 g, F32 b, F32 a /*= 1.f*/)
	{
		m_mainColor = { r, g, b, a };
	}

	void SimpleBrush2D::fillTriangle(const Triangle* tri)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush2D is not ready.");

		float pushConstantsPos[6] = { tri->m_vertices[0].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[0].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[1].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[1].pos.y / m_screenHeight * 2.f - 1.f, tri->m_vertices[2].pos.x / m_screenWidth * 2.f - 1.f, tri->m_vertices[2].pos.y / m_screenHeight * 2.f - 1.f };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_trianglePipeline.getLayout(), VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(float) * 6, pushConstantsPos);

		float pushConstantsColor[4] = { tri->m_vertices[0].color.r, tri->m_vertices[0].color.g, tri->m_vertices[0].color.b, tri->m_vertices[0].color.a };
		vkCmdPushConstants(m_pCmdBuffer->raw(), m_trianglePipeline.getLayout(), VK_SHADER_STAGE_FRAGMENT_BIT, 0, sizeof(float) * 4, pushConstantsColor);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, Triangle::s_vertexBuffer.ptr(), offsets);
		vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), Triangle::s_indexBuffer.raw(), 0, VK_INDEX_TYPE_UINT32);

		vkCmdDrawIndexed(m_pCmdBuffer->raw(), 3, 1, 0, 0, 0);
	}

	void SimpleBrush2D::fillVertexArray(Vertex2DArray& vertexArray)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush2D is not ready.");

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_trianglePipeline.raw());

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

		vkCmdSetViewport(m_pCmdBuffer->raw(), 0, 1, &viewport);
		vkCmdSetScissor(m_pCmdBuffer->raw(), 0, 1, &scissor);
		
		vertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, vertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), vertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush2D::drawVertexArray(Vertex2DArray & vertexArray)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush2D is not ready.");

		if (vertexArray.isEmpty())return;

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_linePipeline.raw());

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

		vkCmdSetViewport(m_pCmdBuffer->raw(), 0, 1, &viewport);
		vkCmdSetScissor(m_pCmdBuffer->raw(), 0, 1, &scissor);

		vertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, vertexArray.m_buffer.ptr(), offsets);
		vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), m_triangleLineIndexBuffer.raw(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(m_pCmdBuffer->raw(), 6, 1, 0, 0, 0);
	}

	void SimpleBrush2D::drawLine(const Vertex2D& start, const Vertex2D& end)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush2D is not ready.");

		m_lineVertexArray.push(start);
		m_lineVertexArray.push(end);
	}

	void SimpleBrush2D::drawLine(F32 x1, F32 y1, F32 x2, F32 y2)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush2D is not ready.");

		m_lineVertexArray.push(Vertex2D({ x1, y1 }, m_mainColor));
		m_lineVertexArray.push(Vertex2D({ x2, y2 }, m_mainColor));
	}

	void SimpleBrush2D::drawPoint(const Vertex2D & pos)
	{
		m_pointVertexArray.push(pos);
	}

	void SimpleBrush2D::drawPoint(F32 x, F32 y)
	{
		m_pointVertexArray.push(Vertex2D({ x, y }, m_mainColor));
	}

	void SimpleBrush2D::create(VK::Vulkan* pVulkan, U32 screenWidth, U32 screenHeight)
	{
		m_pVK = pVulkan;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_vertexShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshadervert.spv");
		m_fragmentShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshaderfrag.spv");


		SimpleBrush2D::createPipeline();

		{
			lw::DynamicArray<U16> indexArrayVec = { 0, 1, 1, 2, 2, 0 };
			VK::StagingBuffer stagingBuffer;
			size_t dataSize = indexArrayVec.size() * sizeof(U16);
			stagingBuffer.allocate(&m_pVK->m_device, dataSize);
			void* dataPtr = stagingBuffer.map();
			memcpy(dataPtr, indexArrayVec.raw(), dataSize);
			stagingBuffer.unmap();
			m_triangleLineIndexBuffer.allocate(&m_pVK->m_device, &m_pVK->m_commandPool, m_pVK->m_device.getGraphicsQueue(), &stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			stagingBuffer.destroy();
		}

		m_pointVertexArray.m_vertices.reserve(9000000);

		Triangle::s_init(&m_pVK->m_device, &m_pVK->m_commandPool);
	}

	void SimpleBrush2D::destroy()
	{
		Triangle::s_deinit();
		

		SimpleBrush2D::destroyPipeline();

		m_fragmentShader.destroy();
		m_vertexShader.destroy();
	}

	void SimpleBrush2D::createPipeline()
	{
		
		m_trianglePipeline.init(m_screenWidth, m_screenHeight);
		m_trianglePipeline.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_trianglePipeline.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_trianglePipeline.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_trianglePipeline.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_trianglePipeline.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_trianglePipeline.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);
		

		m_linePipeline.init(m_screenWidth, m_screenHeight);
		m_linePipeline.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_linePipeline.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_linePipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
		m_linePipeline.setPolygonMode(VK_POLYGON_MODE_LINE);
		m_linePipeline.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_linePipeline.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_linePipeline.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_linePipeline.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);

		m_pointPipeline.init(m_screenWidth, m_screenHeight);
		m_pointPipeline.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_pointPipeline.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_pointPipeline.setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
		m_pointPipeline.setPolygonMode(VK_POLYGON_MODE_POINT);
		m_pointPipeline.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pointPipeline.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_pointPipeline.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_pointPipeline.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);
	}

	void SimpleBrush2D::destroyPipeline()
	{
		m_pointPipeline.destroy();
		m_linePipeline.destroy();
		m_trianglePipeline.destroy();
	}

	void SimpleBrush2D::prepare(const VK::CommandBuffer* cmd)
	{
		m_pCmdBuffer = cmd;

		m_lineVertexArray.clear();
		m_pointVertexArray.clear();


		m_ready = true;
	}

	void SimpleBrush2D::disperse()
	{
		drawAllLines();
		drawAllPoints();


		m_pCmdBuffer = nullptr;
		
		m_ready = false;
	}

	void SimpleBrush2D::resize(U32 screenWidth, U32 screenHeight)
	{
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		destroyPipeline();
		createPipeline();
	}

	void SimpleBrush2D::drawAllLines()
	{
		if (m_lineVertexArray.isEmpty())return;

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_linePipeline.raw());

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

		vkCmdSetViewport(m_pCmdBuffer->raw(), 0, 1, &viewport);
		vkCmdSetScissor(m_pCmdBuffer->raw(), 0, 1, &scissor);

		m_lineVertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, m_lineVertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), m_lineVertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush2D::drawAllPoints()
	{
		if (m_pointVertexArray.isEmpty())return;

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pointPipeline.raw());

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

		vkCmdSetViewport(m_pCmdBuffer->raw(), 0, 1, &viewport);
		vkCmdSetScissor(m_pCmdBuffer->raw(), 0, 1, &scissor);

		m_pointVertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, m_pointVertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), m_pointVertexArray.size(), 1, 0, 0);
	}

}