#pragma once

#include "Standard.hpp"

#include <initializer_list>
#include "Exceptions.hpp"

#define ALLOCATE_STEPS 10

namespace lw
{
	//#TODO use own allocator -> performance


	template<typename T>
	class DynamicArray
	{
		T* m_pData;
		size_t m_size;
		size_t m_capacity;

	public:
		DynamicArray()
		{
			m_pData = nullptr;
			m_size = 0;
			m_capacity = 0;
		}

		DynamicArray(const DynamicArray<T>& d)
		{
			operator=(d);
		}

		DynamicArray(size_t length)
		{
			m_pData = nullptr;
			m_size = length;
			allocate(m_size + ALLOCATE_STEPS);
			for (size_t i = 0; i < m_size; i++)
			{
				m_pData[i] = T();
			}
		}

		DynamicArray(const std::initializer_list<T>& il)
		{
			operator=(il);
		}

		~DynamicArray()
		{
			destroy();
		}

		void operator=(const DynamicArray<T>& d)
		{
			if (m_pData != nullptr)
			{
				delete[] m_pData;
				m_pData = nullptr;
			}
			m_size = d.m_size;
			m_capacity = d.m_capacity;
			m_pData = new T[m_capacity];
			memcpy(m_pData, d.m_pData, sizeof(T) * m_size);
		}

		void operator=(const std::initializer_list<T>& il)
		{
			destroy();
			growIfNeeded(il.size());
			m_size = il.size();
			size_t k = 0;
			for (const T* i = il.begin(); i != il.end(); i++)
			{
				m_pData[k] = *i;
				k++;
			}
		}

		size_t size() const
		{
			return m_size;
		}

		bool isEmpty() const
		{
			return m_size == 0;
		}

		T* raw()
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

		T* begin() //#TODO: use iterators
		{
			return &m_pData[0];
		}

		const T* begin() const
		{
			return &m_pData[0];
		}

		T* end()
		{
			return m_size ? &m_pData[m_size - 1] : &m_pData[0];
		}

		const T* end() const
		{
			return &m_pData[m_size - 1];
		}

		void reserve(size_t memNeeded)
		{
			growIfNeeded(memNeeded);
		}

		void resize(size_t newSize)
		{
			growIfNeeded(newSize);
			m_size = newSize;
		}

		void push(const T& element) //#TODO parameter pack
		{
			growIfNeeded(m_size + 1);
			m_pData[m_size] = element;
			m_size++;
		}

		void push(const T&& element)
		{
			push(element);
		}

		template<class ...Args>
		void emplace(Args... args)
		{
			growIfNeeded(m_size + 1);
			m_pData[m_size] = T(args...);
			m_size++;
		}

		//#TODO check emplace

		void pop()
		{
			m_size--;
		}

		void clear()
		{
			m_size = 0;
		}

	private:
		void destroy()
		{
			delete[] m_pData;
			m_pData = nullptr;
			m_size = 0;
			m_capacity = 0;
		}

		void growIfNeeded(size_t neededCap)
		{
			if (neededCap <= m_capacity)return;
			size_t newCap = neededCap;
			if (m_capacity - newCap < ALLOCATE_STEPS)newCap += ALLOCATE_STEPS;
			allocate(newCap);
		}

		void allocate(size_t newCapacity)
		{
			auto* pNewData = new T[newCapacity];

			if (m_capacity != 0)
			{
				memcpy(pNewData, m_pData, sizeof(T) * m_size);
			}
			delete[] m_pData;

			m_capacity = newCapacity;
			m_pData = pNewData;


		}

	};
}