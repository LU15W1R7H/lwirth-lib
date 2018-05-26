#pragma once

#include "Standard.hpp"

#include "Vertices.hpp"
#include "Shape.hpp"

#include "VulkanBuffer.hpp"

namespace lw
{
	class LWAPI Triangle : public Shape
	{
		friend class SimpleBrush2D;

		static void s_init(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
		static void s_deinit();

		static VK::GPUBuffer s_vertexBuffer;
		static VK::GPUBuffer s_indexBuffer;

	public:
		//vertices shall be counter clockwise
		Triangle(Vertex2D v0, Vertex2D v1, Vertex2D v2);
		~Triangle();
		Triangle* clone() const override;

		Vertex2D m_vertices[3];

		f32 area() const override;
		void rotate(f32 angle, Vec2& center) override;

		

		
	};
}



