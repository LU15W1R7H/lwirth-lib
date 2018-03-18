#pragma once

#include "Standard.hpp"

#include <vector>
#include "Vector2.hpp"
#include "Color.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	class API Vertex
	{
	public:
		Vertex();
		~Vertex();
		Vertex(Vector2 pos, Color color);

		Vector2 pos;
		Color color;
	};

	class API VertexArray
	{
		friend class SimpleBrush2D;
	private:
		bool m_needsUpdate = false;

		std::vector<Vertex> m_vertices;
		VK::GPUBuffer m_buffer;
		VK::StagingBuffer m_stagingBuffer;

		void updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
	public:
		VertexArray(U32 size);
		~VertexArray();

		U32 size() const;
		void resize(U32 newSize);
		Vertex& at(U32 index);
		Vertex& operator[](U32 index);

	};
}



