#pragma once

#include "../Standard.hpp"

#include <initializer_list>
#include "../general/Exceptions.hpp"
#include "../math/Math.hpp"

namespace lw
{
	template<class T, class Allocator = std::allocator<T>>
	class List
	{
	private:

		T* m_pData;
		size_t m_size;
		size_t m_capacity;

		Allocator* m_pAllocator;
		bool m_shouldDestroyAllocator;

	public:
		using iterator = T*;

		List(Allocator* pAllocator = nullptr)
		{
			create(0, pAllocator);
		}

		List(size_t size, Allocator* pAllocator = nullptr)
		{
			create(size, pAllocator);
			construct(size);
		}

		List(const List<T>& other, Allocator* pAllocator = nullptr)
		{
			create(other.m_size, pAllocator);
			m_size = other.m_size;
			for (size_t i = 0; i < m_size; i++)
			{
				new(std::addressof(m_pData[i])) T(other.m_pData[i]);
			}
		}

		List(List<T>&& other, Allocator* pAllocator = nullptr)
		{
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_pData = other.m_pData;
			m_pAllocator = other.m_pAllocator;
			m_shouldDestroyAllocator = other.m_shouldDestroyAllocator;

			other.m_size = 0;
			other.m_capacity = 0;
			other.m_pData = nullptr;
			other.m_pAllocator = nullptr;
			other.m_shouldDestroyAllocator = false;
		}

		List(const std::initializer_list<T>& il, Allocator* pAllocator = nullptr)
		{
			create(il.size(), pAllocator);
			m_size = il.size();
			auto iter = il.begin();
			for (size_t i = 0; i < m_size; i++)
			{
				new(std::addressof(m_pData[i])) T(*iter);
				iter++;
			}
		}

		void operator=(const List<T>& other)
		{
			destroy();

			create(other.m_size, nullptr);
			m_size = other.m_size;
			for (size_t i = 0; i < other.m_size; i++)
			{
				new(std::addressof(m_pData[i])) T(other.m_pData[i]);
			}
		}

		void operator=(List<T>&& other)
		{
			destroy();
			
			m_size = other.m_size;
			m_capacity = other.m_capacity;
			m_pData = other.m_pData;
			m_pAllocator = other.m_pAllocator;
			m_shouldDestroyAllocator = other.m_shouldDestroyAllocator;

			other.m_size = 0;
			other.m_capacity = 0;
			other.m_pData = nullptr;
			other.m_pAllocator = nullptr;
			other.m_shouldDestroyAllocator = false;
		}

		void operator=(const std::initializer_list<T>& il)
		{
			destroy();

			create(il.size(), nullptr);
			m_size = il.size();
			size_t i = 0;
			for (auto iter = il.begin(); iter != il.end(); iter++)
			{
				new(std::addressof(m_pData[i])) T(*iter);
				i++;
			}
		}

		~List()
		{
			destroy();
		}


		size_t size() const
		{
			return m_size;
		}

		size_t capacity() const
		{
			return m_capacity;
		}

		bool isEmpty() const
		{
			return m_size == 0;
		}

		T* raw()
		{
			return m_pData;
		}

		const T* raw() const
		{
			return m_pData;
		}

		T& at(size_t index)
		{
			if (index >= m_size)throw IllegalIndexException("Illegal dynamic array index");
			return m_pData[index];
		}

		const T& at(size_t index) const
		{
			if (index >= m_size)throw IllegalIndexException("Illegal dynamic array index");
			return m_pData[index];
		}

		T& operator[](size_t index)
		{
			return at(index);
		}

		const T& operator[](size_t index) const
		{
			return at(index);
		}
		
		T& front()
		{
			return m_pData[0];
		}

		const T& front() const
		{
			return m_pData[0];
		}

		T& back()
		{
			return m_pData[m_size - 1];
		}

		const T& back() const
		{
			return m_pData[m_size - 1];
		}

		iterator begin()
		{
			return &m_pData[0];
		}

		const iterator begin() const
		{
			return &m_pData[0];
		}

		iterator end()
		{
			return &m_pData[m_size];
		}

		const iterator end() const
		{
			return &m_pData[m_size];
		}

		void reserve(size_t memNeeded)
		{
			if (memNeeded <= m_capacity)return;
			allocate(memNeeded);
		}

		void resize(size_t newSize)
		{
			construct(newSize);
		}

		template<class ... Elements>
		void push(Elements&& ... elements)
		{
			size_t index = m_size;
			provide(m_size + sizeof...(Elements));
			consumePush(index, std::forward<Elements>(elements)...);
		}


		template<class ...Args>
		void emplace(Args... args)
		{
			provide(m_size + 1);
			m_pData[m_size - 1] = T(args...);
		}


		void pop()
		{
			m_size--;
		}

		void clear()
		{
			for (size_t i = 0; i < m_size; i++)
			{
				m_pData[i].~T();
			}
			m_size = 0;
		}

	private:
		void create(size_t capacity, Allocator* allocator)
		{
			m_pData = nullptr;
			m_size = 0;
			m_capacity = 0;

			m_pAllocator = nullptr;
			m_shouldDestroyAllocator = false;


			if (!m_pAllocator)
			{
				m_pAllocator = new Allocator();
				m_shouldDestroyAllocator = true;
			}
			else
			{
				m_pAllocator = allocator;
				m_shouldDestroyAllocator = false;
			}

			allocate(capacity);
		}

		void destroy()
		{
			clear();

			if (m_pData)
			{
				m_pAllocator->deallocate(m_pData, m_capacity);
			}

			if (m_shouldDestroyAllocator)
			{
				delete m_pAllocator;
			}

			m_pData = nullptr;
			m_size = 0;
			m_capacity = 0;
			m_pAllocator = nullptr;
			m_shouldDestroyAllocator = false;
		}

		void allocate(size_t newCapacity)
		{
			if (m_capacity >= newCapacity)return;
			auto* pNewData = m_pAllocator->allocate(newCapacity);

			if (m_pData)
			{
				for (size_t i = 0; i < m_size; i++)
				{
					new (std::addressof(pNewData[i])) T(std::move(m_pData[i]));
					m_pData[i].~T();
				}
				m_pAllocator->deallocate(m_pData, m_capacity);
			}

			m_capacity = newCapacity;
			m_pData = pNewData;
		}

		void provide(size_t newSize)
		{
			if (newSize > m_capacity)
			{
				size_t newCap = lw::max(newSize, m_capacity * 2);
				allocate(newCap);
			}
			m_size = newSize;
		}

		void construct(size_t newSize)
		{
			provide(newSize);
			
			for (size_t i = m_size; i < newSize; i++)
			{
				new(std::addressof(m_pData[i])) T(); //only initializes (doesn't allocate)
			}
		}


		template<typename First, typename ... Rest>
		void consumePush(size_t index, First && first, Rest && ... rest)
		{
			m_pData[index] = static_cast<T>(first);
			consumePush(index + 1, std::forward<Rest>(rest)...);
		}

		//when finished
		void consumePush(size_t index)
		{}

	};
}