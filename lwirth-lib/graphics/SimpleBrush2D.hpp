#pragma once

#include "../Standard.hpp"

#define GLFW_INCLUDE_VULKAN
#include <glfw3.h>

#include "vulkan/VulkanPipeline.hpp"
#include "vulkan/VulkanCommands.hpp"
#include "vulkan/VulkanShader.hpp"
#include "vulkan/VulkanBuffer.hpp"
#include "../math/Triangle.hpp"
#include "Vertices.hpp"

namespace lw
{
	namespace VK
	{
		class Device;
		class Vulkan;
	}

	class SimpleBrush2D
	{
		friend class VK::Vulkan;
	private:
		VK::VertexShader m_vertexShader;
		VK::FragmentShader m_fragmentShader;

		VK::Pipeline m_pipelineLine;
		VK::Pipeline m_pipelinePoint;
		VK::Pipeline m_pipelineTriangleFill;


		u32 m_screenWidth;
		u32 m_screenHeight;

		bool m_ready = false;

		VK::Vulkan* m_pVK = nullptr;
		const VK::CommandBuffer* m_pCmd = nullptr;

		Color m_mainColor;
		VK::GPUBuffer m_triangleMeshIndexBuffer;
		VK::GPUBuffer m_quadFillIndexBuffer;

		Vertex2DArray m_lineVertexArray;
		Vertex2DArray m_pointVertexArray;

	public:
		SimpleBrush2D() = default;
		~SimpleBrush2D() = default;

		void setColor(const Color& color);
		void setColor(f32 r, f32 g, f32 b, f32 a = 1.f);
		void drawVertexArray(Vertex2DArray& vertexArray);
		void drawLine(const Vertex2D& start, const Vertex2D& end);
		void drawLine(f32 x1, f32 y1, f32 x2, f32 y2);
		void drawPoint(const Vertex2D& pos);
		void drawPoint(f32 x, f32 y);

	private:
		void create(VK::Vulkan* pVulkan, u32 screenWidth, u32 screenHeight);
		void destroy();

		void createPipeline();
		void destroyPipeline();

		void prepare(const VK::CommandBuffer* cmdBuffer);
		void disperse();

		void resize(u32 screenWidth, u32 screenHeight);

		void drawAllLines();
		void drawAllPoints();

		void preparePipeline(VK::Pipeline & pipeline);

		void drawVertexArrayTriangleFill(Vertex2DArray & va);
		void drawVertexArrayTriangleMesh(Vertex2DArray & va);
		void drawVertexArrayQuadFill(Vertex2DArray & va);
		void drawVertexArrayQuadMesh(Vertex2DArray & va);
	};
}



