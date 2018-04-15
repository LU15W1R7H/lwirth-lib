#pragma once

#include "Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "VulkanPipeline.hpp"
#include "VulkanCommands.hpp"
#include "VulkanShader.hpp"
#include "VulkanBuffer.hpp"
#include "Triangle.hpp"
#include "Vertices.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class Vulkan;
	}

	class API SimpleBrush3D
	{
		friend class VK::Vulkan;
	private:
		VK::VertexShader m_vertexShader;
		VK::FragmentShader m_fragmentShader;

		VK::Pipeline m_pipelineLine;
		VK::Pipeline m_pipelinePoint;
		VK::Pipeline m_pipelineTriangleFill;


		U32 m_screenWidth;
		U32 m_screenHeight;

		bool m_ready = false;

		VK::Vulkan* m_pVK = nullptr;
		const VK::CommandBuffer* m_pCmdBuffer = nullptr;

		Color m_mainColor;
		VK::GPUBuffer m_triangleMeshIndexBuffer;
		VK::GPUBuffer m_quadFillIndexBuffer;

		Vertex2DArray m_lineVertexArray;
		Vertex2DArray m_pointVertexArray;

	public:
		SimpleBrush3D() = default;
		~SimpleBrush3D() = default;

		void setColor(const Color& color);
		void setColor(F32 r, F32 g, F32 b, F32 a = 1.f);
		void drawVertexArray(Vertex2DArray& vertexArray);
		void drawLine(const Vertex2D& start, const Vertex2D& end);
		void drawLine(F32 x1, F32 y1, F32 x2, F32 y2);
		void drawPoint(const Vertex2D& pos);
		void drawPoint(F32 x, F32 y);

	private:
		void create(VK::Vulkan* pVulkan, U32 screenWidth, U32 screenHeight);
		void destroy();

		void createPipeline();
		void destroyPipeline();

		void prepare(const VK::CommandBuffer* cmdBuffer);
		void disperse();

		void resize(U32 screenWidth, U32 screenHeight);

		void drawAllLines();
		void drawAllPoints();

		void preparePipeline(VK::Pipeline & pipeline);

		void drawVertexArrayTriangleFill(Vertex2DArray & va);
		void drawVertexArrayTriangleMesh(Vertex2DArray & va);
		void drawVertexArrayQuadFill(Vertex2DArray & va);
		void drawVertexArrayQuadMesh(Vertex2DArray & va);
	};
}



