#pragma once

#include "Standard.hpp"

#include "DynamicArray.hpp"
#include "Color.hpp"
#include "Vector.hpp"
#include "VulkanBuffer.hpp"
#include "VulkanDevice.hpp"
#include "VulkanCommands.hpp"

namespace lw
{
	class API Vertex2D
	{
	public:
		Vertex2D();
		~Vertex2D() = default;
		Vertex2D(Vec2 pos, Color color);

		Vec2 pos;
		Color color;
	};

	class API Vertex3D
	{
	public:
		Vertex3D();
		~Vertex3D() = default;
		Vertex3D(Vec3 pos, Color color);

		Vec3 pos;
		Color color;
	};


	class API Vertex2DArray
	{
		friend class SimpleBrush2D;
	private:
		bool m_needsUpdate = false;

		lw::DynamicArray<Vertex2D> m_vertices;
		VK::GPUBuffer m_buffer;
		VK::StagingBuffer m_stagingBuffer;

		void updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
	public:
		Vertex2DArray();
		Vertex2DArray(U32 size);
		~Vertex2DArray() = default;

		U32 size() const;
		bool isEmpty() const;
		void resize(U32 newSize);
		Vertex2D& at(U32 index);
		Vertex2D& operator[](U32 index);
		void push(const Vertex2D&);
		void push(const Vertex2D&&);
		void clear();
	};

	class API Vertex3DArray
	{
		friend class SimpleBrush3D;
	private:
		bool m_needsUpdate = false;

		lw::DynamicArray<Vertex3D> m_vertices;
		VK::GPUBuffer m_buffer;
		VK::StagingBuffer m_stagingBuffer;

		void updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool);
	public:
		Vertex3DArray(U32 size);
		~Vertex3DArray();

		U32 size() const;
		void resize(U32 newSize);
		Vertex3D& at(U32 index);
		Vertex3D& operator[](U32 index);

	};
}



