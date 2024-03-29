#include "Triangle.hpp"

#include <math.h>
#include <cstring>
#include "../graphics/vulkan/VulkanDevice.hpp"

namespace lw
{
	VK::GPUBuffer Triangle::s_vertexBuffer;
	VK::GPUBuffer Triangle::s_indexBuffer;

	void Triangle::s_init(const VK::Device* pDevice, const VK::CommandPool* pCommandPool)
	{
		{
			VK::StagingBuffer staging;
			staging.allocate(pDevice, sizeof(float) * 6);
			float data[] = { 1, 1, 1, 1, 1, 1 };
			void* dataPtr = staging.map();
			std::memcpy(dataPtr, data, sizeof(Vertex2D) * 3);
			staging.unmap();
			s_vertexBuffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &staging, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			staging.destroy();
		}

		{
			VK::StagingBuffer staging;
			staging.allocate(pDevice, sizeof(u32) * 3);
			u32 data[] = { 0, 1, 2 };
			void* dataPtr = staging.map();
			std::memcpy(dataPtr, data, sizeof(u32) * 3);
			staging.unmap();
			s_indexBuffer.allocate(pDevice, pCommandPool, pDevice->getGraphicsQueue(), &staging, VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_SHARING_MODE_EXCLUSIVE);
			staging.destroy();
		}
	}

	void Triangle::s_deinit()
	{
		s_indexBuffer.destroy();
		s_vertexBuffer.destroy();
	}

	Triangle::Triangle(Vertex2D v0, Vertex2D v1, Vertex2D v2)
		: Shape(Vec2(0, 0))
	{
		m_vertices[0] = v0;
		m_vertices[1] = v1;
		m_vertices[2] = v2;

	}


	Triangle::~Triangle()
	{
	}

	Triangle* Triangle::clone() const
	{
		return new Triangle(*this);
	}

	f32 Triangle::area() const
	{
		return 0.5f * std::abs((m_vertices[0].pos.x() - m_vertices[2].pos.x()) * (m_vertices[1].pos.y() - m_vertices[0].pos.y()) - (m_vertices[0].pos.x() - m_vertices[1].pos.x()) * (m_vertices[2].pos.y() - m_vertices[0].pos.y()));
	}

	void Triangle::rotate(f32 angle, Vec2 & center)
	{
	}

	/*void Triangle::initRendering(VK::Device* pDevice, VK::CommandPool* commandPool, VkQueue queue, VK::QueueFamilyIndices* indices)
	{
		s_vertexBuffer.init(pDevice, sizeof(float) * 6, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices, VK_SHARING_MODE_EXCLUSIVE);
		float data[] =
		{
			0, 0,
			1, 0,
			0.5f, 1
		};

		void* dataBuf = s_vertexBuffer.map();
		memcpy(dataBuf, data, sizeof(float) * 6);
		s_vertexBuffer.unmap();
		s_vertexBuffer.upload(commandPool, queue);

		s_indexBuffer.init(pDevice, sizeof(U32) * 3, VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, indices, VK_SHARING_MODE_EXCLUSIVE);

		U32 data2[] = {
			0, 1, 2
		};
		void* dataBuf2 = s_indexBuffer.map();
		memcpy(dataBuf2, data2, sizeof(uint32_t) * 3);
		s_indexBuffer.unmap();

	}*/

	

	//Triangle Triangle::createEquilateral(Vec2 pos, F32 sideLength)
	//{
	//	
	//	/*return Triangle({
	//		Vec2(0.f, std::sqrtf(3.f) / 3.f * sideLength),
	//		Vec2(-sideLength/2.f, -std::sqrtf(3.f) / 6.f * sideLength),
	//		Vec2(sideLength / 2.f, -std::sqrtf(3.f) / 6.f * sideLength)
	//	});*/
	//}

	//Triangle Triangle::createIsosceles(Vec2 pos, F32 baseLenght, F32 scelesLength)
	//{
	//	/*return Triangle(pos, {
	//		Vec2(0.f, std::sqrtf(3.f) / 3.f * scelesLength),
	//		Vec2(-baseLenght / 2.f, -std::sqrtf(3.f) / 6.f * scelesLength),
	//		Vec2(baseLenght / 2.f, -std::sqrtf(3.f) / 6.f * scelesLength)
	//	});*/
	//}



}


