#include "stdafx.hpp"
#include "SimpleBrush3D.hpp"

#include "Color.hpp"

#include "Vulkan.hpp"


namespace lw
{

	void SimpleBrush3D::setColor(const Color& color)
	{
		m_mainColor = color;
	}

	void SimpleBrush3D::setColor(F32 r, F32 g, F32 b, F32 a /*= 1.f*/)
	{
		m_mainColor = { r, g, b, a };
	}



	void SimpleBrush3D::drawVertexArray(Vertex2DArray & va)
	{
		/*switch (va.m_primitive)
		{
		case Triangles:
			drawVertexArrayTriangleFill(va);
			break;
		case Quads:
			drawVertexArrayQuadFill(va);
			break;
		default:
			throw std::logic_error("not available");
		}*/
	}

	void SimpleBrush3D::drawLine(const Vertex2D& start, const Vertex2D& end)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");

		m_lineVertexArray.push(start);
		m_lineVertexArray.push(end);
	}

	void SimpleBrush3D::drawLine(F32 x1, F32 y1, F32 x2, F32 y2)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");

		m_lineVertexArray.push(Vertex2D({ x1, y1 }, m_mainColor));
		m_lineVertexArray.push(Vertex2D({ x2, y2 }, m_mainColor));
	}

	void SimpleBrush3D::drawPoint(const Vertex2D & pos)
	{
		m_pointVertexArray.push(pos);
	}

	void SimpleBrush3D::drawPoint(F32 x, F32 y)
	{
		m_pointVertexArray.push(Vertex2D({ x, y }, m_mainColor));
	}

	void SimpleBrush3D::create(VK::Vulkan* pVulkan, U32 screenWidth, U32 screenHeight)
	{
		m_pVK = pVulkan;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_vertexShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshadervert.spv");
		m_fragmentShader.create(&m_pVK->m_device, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshaderfrag.spv");


		SimpleBrush3D::createPipeline();

		//prepare index buffers
		{
			lw::DynamicArray<U16> indexArrayVec = { 0, 1, 1, 2, 2, 0 };
			VK::StagingBuffer stagingBuffer;
			size_t dataSize = indexArrayVec.size() * sizeof(U16);
			stagingBuffer.allocate(&m_pVK->m_device, dataSize);
			void* dataPtr = stagingBuffer.map();
			memcpy(dataPtr, indexArrayVec.raw(), dataSize);
			stagingBuffer.unmap();
			m_triangleMeshIndexBuffer.allocate(&m_pVK->m_device, &m_pVK->m_commandPool, m_pVK->m_device.getGraphicsQueue(), &stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			stagingBuffer.destroy();
		}

		{
			lw::DynamicArray<U16> indexArrayVec = { 0, 1, 2, 2, 3, 0 };
			VK::StagingBuffer stagingBuffer;
			size_t dataSize = indexArrayVec.size() * sizeof(U16);
			stagingBuffer.allocate(&m_pVK->m_device, dataSize);
			void* dataPtr = stagingBuffer.map();
			memcpy(dataPtr, indexArrayVec.raw(), dataSize);
			stagingBuffer.unmap();
			m_quadFillIndexBuffer.allocate(&m_pVK->m_device, &m_pVK->m_commandPool, m_pVK->m_device.getGraphicsQueue(), &stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			stagingBuffer.destroy();
		}

		//Triangle::s_init(&m_pVK->m_device, &m_pVK->m_commandPool);
	}

	void SimpleBrush3D::destroy()
	{
		//Triangle::s_deinit();


		SimpleBrush3D::destroyPipeline();

		m_fragmentShader.destroy();
		m_vertexShader.destroy();
	}

	void SimpleBrush3D::createPipeline()
	{

		m_pipelinePoint.init(m_screenWidth, m_screenHeight);
		m_pipelinePoint.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_pipelinePoint.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_pipelinePoint.setTopology(VK_PRIMITIVE_TOPOLOGY_POINT_LIST);
		m_pipelinePoint.setPolygonMode(VK_POLYGON_MODE_POINT);
		m_pipelinePoint.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pipelinePoint.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_pipelinePoint.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_pipelinePoint.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);

		m_pipelineLine.init(m_screenWidth, m_screenHeight);
		m_pipelineLine.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_pipelineLine.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_pipelineLine.setTopology(VK_PRIMITIVE_TOPOLOGY_LINE_LIST);
		m_pipelineLine.setPolygonMode(VK_POLYGON_MODE_LINE);
		m_pipelineLine.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pipelineLine.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_pipelineLine.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_pipelineLine.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);


		m_pipelineTriangleFill.init(m_screenWidth, m_screenHeight);
		m_pipelineTriangleFill.addDynamicState(VK_DYNAMIC_STATE_VIEWPORT);
		m_pipelineTriangleFill.addDynamicState(VK_DYNAMIC_STATE_SCISSOR);
		m_pipelineTriangleFill.setTopology(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
		m_pipelineTriangleFill.setPolygonMode(VK_POLYGON_MODE_FILL);
		m_pipelineTriangleFill.addVertexBinding(0, sizeof(Vertex2D), VK_VERTEX_INPUT_RATE_VERTEX);
		m_pipelineTriangleFill.addVertexDescription(0, 0, VK_FORMAT_R32G32_SFLOAT, offsetof(Vertex2D, Vertex2D::pos));
		m_pipelineTriangleFill.addVertexDescription(1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(Vertex2D, Vertex2D::color));
		m_pipelineTriangleFill.create(&m_pVK->m_device, &m_pVK->m_renderPass, &m_vertexShader, &m_fragmentShader);

	}

	void SimpleBrush3D::destroyPipeline()
	{
		m_pipelineTriangleFill.destroy();
		m_pipelineLine.destroy();
		m_pipelinePoint.destroy();
	}

	void SimpleBrush3D::prepare(const VK::CommandBuffer* cmd)
	{
		m_pCmdBuffer = cmd;

		m_lineVertexArray.clear();
		m_pointVertexArray.clear();


		m_ready = true;
	}

	void SimpleBrush3D::disperse()
	{
		drawAllLines();
		drawAllPoints();


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

	void SimpleBrush3D::drawAllLines()
	{
		if (m_lineVertexArray.isEmpty())return;

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelineLine.raw());

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

		//m_lineVertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, m_lineVertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), m_lineVertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush3D::drawAllPoints()
	{
		if (m_pointVertexArray.isEmpty())return;

		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, m_pipelinePoint.raw());

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

		//m_pointVertexArray.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, m_pointVertexArray.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), m_pointVertexArray.size(), 1, 0, 0);
	}

	void SimpleBrush3D::preparePipeline(VK::Pipeline & pipeline)
	{
		vkCmdBindPipeline(m_pCmdBuffer->raw(), VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline.raw());

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
	}

	void SimpleBrush3D::drawVertexArrayTriangleFill(Vertex2DArray & va)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");
		if (va.isEmpty())return;

		preparePipeline(m_pipelineTriangleFill);

		//va.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, va.m_buffer.ptr(), offsets);
		vkCmdDraw(m_pCmdBuffer->raw(), va.size(), 1, 0, 0);
	}


	void SimpleBrush3D::drawVertexArrayTriangleMesh(Vertex2DArray & va)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");
		if (va.isEmpty())return;

		preparePipeline(m_pipelineLine);

		//va.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, va.m_buffer.ptr(), offsets);
		vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), m_triangleMeshIndexBuffer.raw(), 0, VK_INDEX_TYPE_UINT16);
		vkCmdDrawIndexed(m_pCmdBuffer->raw(), 6, 1, 0, 0, 0);
	}

	void SimpleBrush3D::drawVertexArrayQuadFill(Vertex2DArray & va)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");
		if (va.isEmpty())return;

		preparePipeline(m_pipelineTriangleFill);

		//va.updateBuffer(&m_pVK->m_device, &m_pVK->m_commandPool);

		VkDeviceSize offsets[] = { 0 };
		//vkCmdBindVertexBuffers(m_pCmdBuffer->raw(), 0, 1, va.m_buffer.ptr(), offsets);
		vkCmdBindIndexBuffer(m_pCmdBuffer->raw(), m_quadFillIndexBuffer.raw(), 0, VK_INDEX_TYPE_UINT16);
		for (size_t i = 0; i < va.size(); i += 4)
		{
			//#TODO optimize
			vkCmdDrawIndexed(m_pCmdBuffer->raw(), 6, 1, 0, i, 0);
		}

	}

	void SimpleBrush3D::drawVertexArrayQuadMesh(Vertex2DArray & va)
	{

	}
}