#pragma once

#include "Standard.hpp"

#include <memory>

namespace lw
{
	template<class T>
	union PoolChunk
	{
		T obj;
		PoolChunk* next;

		~PoolChunk() = delete;
	};

	template<class T, class Allocator = std::allocator<PoolChunk<T>>>
	class Pool
	{
		using value_type = T;
		using pointer = T * ;
		using const_pointer = const T*;
		using reference = T & ;
		using const_reference = const T&;
		using size_type = size_t;
		using difference_type = std::ptrdiff_t;

	private:
		//PoolAllocatorDestroyer class

		static constexpr const size_t DEFAULT_POOL_SIZE = 1024;

		PoolChunk<T>* m_data = nullptr;
		PoolChunk<T>* m_head = nullptr;
		size_t m_length;

		Allocator* m_allocator = nullptr;
		bool m_shouldDestroyAllocator = false;

	public:
		explicit Pool(size_t size = DEFAULT_POOL_SIZE, Allocator* allocator = nullptr)
			: m_length(size), m_allocator(allocator)
		{
			if (!m_allocator)
			{
				m_allocator = new Allocator();
				m_shouldDestroyAllocator = true;
			}
			else
			{
				m_shouldDestroyAllocator = false;
			}

			m_data = m_allocator->allocate(m_length);
			for (size_t i = 0; i < m_length - 1; i++)
			{
				m_data[i].next = std::addressof(m_data[i + 1]);
			}
			m_data[m_length - 1].next = nullptr;
			m_head = m_data;
		}

		DISABLE_COP_MOV(Pool);

		~Pool()
		{
			if (m_head != m_data)
			{
				clear();
			}

			if (m_data && m_allocator)
			{
				m_allocator->deallocate(m_data, m_length);
			}
			if (m_shouldDestroyAllocator)
			{
				delete m_allocator;
			}

			m_data = nullptr;
			m_head = nullptr;
		}

		template<class... Args>
		T* allocateObject(Args&&... args)
		{
			if (m_head == nullptr)
			{
				debugBreak();
				throw AllocatorOutOfMemoryException();
			}

			PoolChunk<T>* ptr = m_head;
			m_head = ptr->next;
			T* ret = new (ptr) T(std::forward<Args>(args)...);
			return ret;
		}

		void deallocate(T* data)
		{
			if (data < reinterpret_cast<T*>(m_data) ||
				data > reinterpret_cast<T*>(m_data) + m_length)
			{
				throw MalformedPointerException();
			}
			data->~T();
			PoolChunk<T>* chunk = reinterpret_cast<PoolChunk<T>*>(data);
			chunk->next = m_head;
			m_head = chunk;
		}

		void clear()
		{
			for (PoolChunk<T>* p = m_data; p <= m_head; p++)
			{
				p->obj.~T();
			}
			m_head = m_data;
		}
	};
}
