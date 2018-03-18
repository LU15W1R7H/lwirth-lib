#include "stdafx.hpp"
#include "Vertices.hpp"

#include "Exceptions.hpp"

namespace lw
{
	Vertex::Vertex()
		: pos()
	{
	}

	Vertex::~Vertex()
	{
	}

	Vertex::Vertex(Vector2 pos, Color color)
		: pos(pos), color(color)
	{
	}

	VertexArray::VertexArray(U32 size)
	{
		resize(size);
	}

	VertexArray::~VertexArray()
	{
	}

	U32 VertexArray::size() const
	{
		return m_vertices.size();
	}

	void VertexArray::resize(U32 newSize)
	{
		m_vertices.resize(newSize);
	}

	Vertex& VertexArray::at(U32 index)
	{
		if (index >= size())throw IllegalIndexException();
		m_needsUpdate = true;
		return m_vertices[index];
	}

	Vertex& VertexArray::operator[](U32 index)
	{
		return at(index);
	}

	void VertexArray::updateBuffer(const VK::Device* pDevice, const VK::CommandPool* pCommandPool)
	{
		if (!m_needsUpdate)return;

		

		if (m_stagingBuffer.size() != sizeof(Vertex) * size())
		{
			m_stagingBuffer.destroy();
			m_stagingBuffer.allocate(pDevice, sizeof(Vertex) * size());
		}

		void* dataPtr = m_stagingBuffer.map();
		memcpy(dataPtr, m_vertices.data(), sizeof(Vertex) * size());
		m_stagingBuffer.unmap();

		m_buffer.destroy();
		m_buffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &m_stagingBuffer, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);

		m_needsUpdate = false;
	}

}