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

		bool m_ready = false;

		VK::Vulkan* m_pVK = nullptr;
		const VK::CommandBuffer* m_pCmdBuffer = nullptr;

	public:
		void fillTriangle(const Triangle* tri);
		void fillVertexArray(Vertex3DArray& vertexArray);


	private:
		void create(VK::Vulkan* pVulkan, U32 screenWidth, U32 screenHeight);
		void destroy();

		void createPipeline();
		void destroyPipeline();

		void prepare(const VK::CommandBuffer* cmdBuffer);
		void disperse();

		void resize(U32 screenWidth, U32 screenHeight);
	};
}



