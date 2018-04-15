#include "stdafx.hpp"
#include "Vertices.hpp"

#include "Exceptions.hpp"

namespace lw
{
	Vertex2D::Vertex2D()
		: pos()
	{
	}


	Vertex2D::Vertex2D(Vec2 pos, Color color)
		: pos(pos), color(color)
	{
	}

	Vertex2DArray::Vertex2DArray(U32 size, PrimitiveType primitive)
	{
		m_primitive = primitive;
		resize(size);
	}

	Vertex2DArray::Vertex2DArray()
	{
		m_primitive = Lines;
		resize(0);
	}

	U32 Vertex2DArray::size() const
	{
		return m_vertices.size();
	}

	bool Vertex2DArray::isEmpty() const
	{
		return m_vertices.isEmpty();
	}

	void Vertex2DArray::resize(U32 newSize)
	{
		m_vertices.resize(newSize);
	}

	Vertex2D& Vertex2DArray::at(U32 index)
	{
		if (index >= size())throw IllegalIndexException();
		m_needsUpdate = true;
		return m_vertices[index];
	}

	Vertex2D& Vertex2DArray::operator[](U32 index)
	{
		return at(index);
	}

	void Vertex2DArray::push(const Vertex2D & vertex)
	{
		m_needsUpdate = true;
		m_vertices.push(vertex);
	}

	void Vertex2DArray::push(const Vertex2D && vertex)
	{
		m_needsUpdate = true;
		m_vertices.push(vertex);
	}

	void Vertex2DArray::clear()
	{
		m_vertices.clear();
		m_buffer.destroy();
		m_needsUpdate = true;
	}

	void Vertex2DArray::setPrimitiveType(PrimitiveType primitive)
	{
		m_primitive = primitive;
	}

	void Vertex2DArray::updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool)
	{
		if (!m_needsUpdate)return;

		

		if (m_stagingBuffer.size() != sizeof(Vertex2D) * size())
		{
			m_stagingBuffer.destroy();
			m_stagingBuffer.allocate(pDevice, sizeof(Vertex2D) * size());
		}

		void* dataPtr = m_stagingBuffer.map();
		memcpy(dataPtr, m_vertices.raw(), sizeof(Vertex2D) * size());
		m_stagingBuffer.unmap();

		m_buffer.destroy();
		m_buffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &m_stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		m_needsUpdate = false;
	}

	Vertex3D::Vertex3D()
		: pos()
	{
	}

	Vertex3D::Vertex3D(Vec3 pos, Color color)
		: pos(pos), color(color)
	{
	}

	Vertex3DArray::Vertex3DArray(U32 size)
	{
		resize(size);
	}

	U32 Vertex3DArray::size() const
	{
		return m_vertices.size();
	}

	void Vertex3DArray::resize(U32 newSize)
	{
		m_vertices.resize(newSize);
	}

	Vertex3D& Vertex3DArray::at(U32 index)
	{
		if (index >= size())throw IllegalIndexException();
		m_needsUpdate = true;
		return m_vertices[index];
	}

	Vertex3D& Vertex3DArray::operator[](U32 index)
	{
		return at(index);
	}

	void Vertex3DArray::updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool)
	{
		if (!m_needsUpdate)return;



		if (m_stagingBuffer.size() != sizeof(Vertex3D) * size())
		{
			m_stagingBuffer.destroy();
			m_stagingBuffer.allocate(pDevice, sizeof(Vertex3D) * size());
		}

		void* dataPtr = m_stagingBuffer.map();
		memcpy(dataPtr, m_vertices.raw(), sizeof(Vertex3D) * size());
		m_stagingBuffer.unmap();

		m_buffer.destroy();
		m_buffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &m_stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		m_needsUpdate = false;
	}

}