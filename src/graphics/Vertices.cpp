#include "Vertices.hpp"

#include <cstring>

#include "../general/Exceptions.hpp"

namespace lw
{
	Vertex2D::Vertex2D()
		: pos(), color(), texCoord()
	{
	}


	Vertex2D::Vertex2D(Eigen::Vector2f pos, Color color, Vec2 texCoord)
		: pos(pos), color(color), texCoord(texCoord)
	{
	}

	Vertex2D::Vertex2D(Eigen::Vector2f pos, Color color)
		: pos(pos), color(color), texCoord()
	{
	}

	Vertex2D::Vertex2D(Eigen::Vector2f pos, Vec2 texCoord)
		: pos(pos), color(), texCoord(texCoord)
	{
	}

	Vertex2DArray::Vertex2DArray(u32 size, PrimitiveType primitive)
	{
		m_primitive = primitive;
		resize(size);
	}

	Vertex2DArray::Vertex2DArray(const std::initializer_list<Vertex2D>& initList)
	{
		m_vertices = initList;
	}

	Vertex2DArray::Vertex2DArray()
	{
		m_primitive = Lines;
		resize(0);
	}

	u32 Vertex2DArray::size() const
	{
		return m_vertices.size();
	}

	bool Vertex2DArray::isEmpty() const
	{
		return m_vertices.isEmpty();
	}

	void Vertex2DArray::resize(u32 newSize)
	{
		m_vertices.reconstruct(newSize);
	}

	Vertex2D& Vertex2DArray::at(u32 index)
	{
		if (index >= size())throw IllegalIndexException();
		m_needsUpdate = true;
		return m_vertices[index];
	}

	Vertex2D& Vertex2DArray::operator[](u32 index)
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
		std::memcpy(dataPtr, m_vertices.raw(), sizeof(Vertex2D) * size());
		m_stagingBuffer.unmap();

		m_buffer.destroy();
		m_buffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &m_stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		m_needsUpdate = false;
	}

	Vertex3D::Vertex3D()
		: pos(), color(), texCoord()
	{
	}

	Vertex3D::Vertex3D(Eigen::Vector3f pos, Color color, Vec2 texCoord)
		: pos(pos), color(color), texCoord(texCoord)
	{
	}

	Vertex3D::Vertex3D(Eigen::Vector3f pos, Color color)
		: pos(pos), color(color), texCoord()
	{
	}

	Vertex3D::Vertex3D(Eigen::Vector3f pos, Vec2 texCoord)
		: pos(pos), color(), texCoord(texCoord)
	{
	}

	Vertex3DArray::Vertex3DArray(u32 size)
	{
		resize(size);
	}

	u32 Vertex3DArray::size() const
	{
		return m_vertices.size();
	}

	void Vertex3DArray::resize(u32 newSize)
	{
		m_vertices.reconstruct(newSize);
	}

	Vertex3D& Vertex3DArray::at(u32 index)
	{
		if (index >= size())throw IllegalIndexException();
		m_needsUpdate = true;
		return m_vertices[index];
	}

	Vertex3D& Vertex3DArray::operator[](u32 index)
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
		std::memcpy(dataPtr, m_vertices.raw(), sizeof(Vertex3D) * size());
		m_stagingBuffer.unmap();

		m_buffer.destroy();
		m_buffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &m_stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		m_needsUpdate = false;
	}

}