#pragma once

#include "Standard.hpp"

#include "Vertices.hpp"
#include "Shape.hpp"

#include "VulkanBuffer.hpp"

namespace lw
{
	class API Triangle : public Shape
	{
		friend class SimpleBrush2D;

		static void s_init(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
		static void s_deinit();

		static VK::GPUBuffer s_vertexBuffer;
		static VK::GPUBuffer s_indexBuffer;

	public:
		//vertices shall be counter clockwise
		Triangle(Vertex v0, Vertex v1, Vertex v2);
		~Triangle();
		Triangle* clone() const override;

		Vertex m_vertices[3];

		F32 area() const override;
		void rotate(F32 angle, Vector2& center) override;

		

		
	};
}



