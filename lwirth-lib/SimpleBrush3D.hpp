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

#include "Random.hpp"

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
		VK::Pipeline m_pipeline;
		VK::VertexShader m_vertexShader;
		VK::FragmentShader m_fragmentShader;

		U32 m_screenWidth;
		U32 m_screenHeight;

		const VK::Device* m_pDevice = nullptr;
		const VK::CommandBuffer* m_pCmdBuffer = nullptr;
		const VK::CommandPool* m_pCommandPool = nullptr;

	public:
		void fillTriangle(const Triangle* tri);
		void fillVertexArray(Vertex3DArray& vertexArray);

	private:
		void create(const VK::Device* pDevice, const VK::RenderPass* pRenderPass, const VK::CommandPool* pCommandPool, U32 screenWidth, U32 screenHeight);
		void destroy();

		void createPipeline(const VK::RenderPass* pRenderPass);
		void destroyPipeline();

		void prepareDrawing(const VK::CommandBuffer* cmdBuffer);

	};
}



