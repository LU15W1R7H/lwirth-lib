#pragma once

#include "Standard.hpp"

#include "List.hpp"
#include "Color.hpp"
#include "Vector.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	enum PrimitiveType { Points, Lines, LineStrip, Triangles, TriangleStrip, TriangleFan, Quads };

	class Vertex2D
	{
	public:
		Vertex2D();
		~Vertex2D() = default;
		Vertex2D(Vec2 pos, Color color, Vec2 texCoord);
		Vertex2D(Vec2 pos, Color color);
		Vertex2D(Vec2 pos, Vec2 texCoord);

		Vec2 pos;
		Color color;
		Vec2 texCoord;
	};

	class Vertex3D
	{
	public:
		Vertex3D();
		~Vertex3D() = default;
		Vertex3D(Vec3 pos, Color color, Vec2 texCoord);
		Vertex3D(Vec3 pos, Color color);
		Vertex3D(Vec3 pos, Vec2 texCoord);

		Vec3 pos;
		Color color;
		Vec2 texCoord;
	};

	//#TODO: Use special allocator, not dynamic allocation... especially for drawing lines with SimpleBrush2D
	class Vertex2DArray
	{
		friend class SimpleBrush2D;
	private:
		bool m_needsUpdate = false;

		lw::List<Vertex2D> m_vertices;
		VK::GPUBuffer m_buffer;
		VK::StagingBuffer m_stagingBuffer;

		PrimitiveType m_primitive;

		void updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
	public:
		Vertex2DArray();
		Vertex2DArray(u32 size, PrimitiveType primitive);
		~Vertex2DArray() = default;

		u32 size() const;
		bool isEmpty() const;
		void resize(u32 newSize);
		Vertex2D& at(u32 index);
		Vertex2D& operator[](u32 index);
		void push(const Vertex2D&);
		void push(const Vertex2D&&);
		void clear();

		void setPrimitiveType(PrimitiveType primitive);
	};

	class Vertex3DArray
	{
		friend class SimpleBrush3D;
	private:
		bool m_needsUpdate = false;

		lw::List<Vertex3D> m_vertices;
		VK::GPUBuffer m_buffer;
		VK::StagingBuffer m_stagingBuffer;

		void updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
	public:
		Vertex3DArray(u32 size);
		~Vertex3DArray() = default;

		u32 size() const;
		void resize(u32 newSize);
		Vertex3D& at(u32 index);
		Vertex3D& operator[](u32 index);

	};
}



