#include "../stdafx.hpp"
#include "SimpleBrush3D.hpp"

#include "Color.hpp"
#include <cstring>

#include "Vulkan.hpp"


namespace lw
{

	void SimpleBrush3D::setColor(const Color& color)
	{
		m_mainColor = color;
	}

	void SimpleBrush3D::setColor(f32 r, f32 g, f32 b, f32 a /*= 1.f*/)
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

	void SimpleBrush3D::drawLine(f32 x1, f32 y1, f32 x2, f32 y2)
	{
		if (!m_ready)throw NotReadyException("SimpleBrush3D is not ready.");

		m_lineVertexArray.push(Vertex2D({ x1, y1 }, m_mainColor));
		m_lineVertexArray.push(Vertex2D({ x2, y2 }, m_mainColor));
	}

	void SimpleBrush3D::drawPoint(const Vertex2D & pos)
	{
		m_pointVertexArray.push(pos);
	}

	void SimpleBrush3D::drawPoint(f32 x, f32 y)
	{
		m_pointVertexArray.push(Vertex2D({ x, y }, m_mainColor));
	}

	void SimpleBrush3D::create(VK::Vulkan* pVulkan, u32 screenWidth, u32 screenHeight)
	{
		m_pVK = pVulkan;
		m_screenWidth = screenWidth;
		m_screenHeight = screenHeight;

		m_vertexShader.create(&m_pVK->m_mainDevice, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshadervert.spv");
		m_fragmentShader.create(&m_pVK->m_mainDevice, "D:/Dev/C++/My Projects/lwirth-lib/res/shaders/2Dshaderfrag.spv");


		SimpleBrush3D::createPipeline();

		//prepare index buffers
		{
			lw::List<u16> indexArrayVec = { 0, 1, 1, 2, 2, 0 };
			VK::StagingBuffer stagingBuffer;
			size_t dataSize = indexArrayVec.size() * sizeof(u16);
			stagingBuffer.allocate(&m_pVK->m_mainDevice, dataSize);
			void* dataPtr = stagingBuffer.map();
			std::memcpy(dataPtr, indexArrayVec.raw(), dataSize);
			stagingBuffer.unmap();
			m_triangleMeshIndexBuffer.allocate(&m_pVK->m_mainDevice, &m_pVK->m_commandPool, m_pVK->m_mainDevice.getGraphicsQueue(), &stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			stagingBuffer.destroy();
		}

		{
			lw::List<u16> indexArrayVec = { 0, 1, 2, 2, 3, 0 };
			VK::StagingBuffer stagingBuffer;
			size_t dataSize = indexArrayVec.size() * sizeof(u16);
			stagingBuffer.allocate(&m_pVK->m_mainDevice, dataSize);
			void* dataPtr = stagingBuffer.map();
			std::memcpy(dataPtr, indexArrayVec.raw(), dataSize);
			stagingBuffer.unmap();
			m_quadFillIndexBuffer.allocate(&m_pVK->m_mainDevice, &m_pVK->m_commandPool, m_pVK->m_mainDevice.getGraphicsQueue(), &stagingBuffer, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
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

	void SimpleBrush3D::resize(u32 screenWidth, u32 screenHeight)
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
		viewport.width = static_cast<f32>(m_screenWidth);
		viewport.height = static_cast<f32>(m_screenHeight);
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
		viewport.width = static_cast<f32>(m_screenWidth);
		viewport.height = static_cast<f32>(m_screenHeight);
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
		viewport.width = static_cast<f32>(m_screenWidth);
		viewport.height = static_cast<f32>(m_screenHeight);
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