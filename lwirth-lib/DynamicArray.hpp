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
		DynamicArray();
		DynamicArray(const DynamicArray<T>& d);
		DynamicArray(size_t length);
		DynamicArray(const std::initializer_list<T>& il);
		~DynamicArray();

		void operator=(const DynamicArray<T>& d);
		void operator=(const std::initializer_list<T>& il);

		size_t size() const;
		bool isEmpty() const;
		T* raw();
		T& at(size_t index);
		const T& at(size_t index) const;
		T& operator[](size_t index);
		const T& operator[](size_t index) const;
		T& front();
		const T& front() const;
		T& back();
		const T& back() const;
		T* begin(); //#TODO: use iterators
		const T* begin() const; //#TODO: use iterators
		T* end();
		const T* end() const;
		DynamicArray& resize(size_t newSize);
		DynamicArray& push(const T& element);
		DynamicArray& push(const T&& element);
		DynamicArray& pop();
		DynamicArray& clear();

	private:
		void destroy();
		void growIfNeeded(size_t addedElemsCount);
		void allocate(size_t newAllocationsCount);
	};

	template<typename T>
	inline DynamicArray<T>::DynamicArray()
	{
		m_pData = nullptr;
		m_size = 0;
		m_capacity = 0;
		allocate(ALLOCATE_STEPS);
	}

	template<typename T>
	inline DynamicArray<T>::DynamicArray(const DynamicArray<T>& d)
	{
		operator=(d);
	}

	template<typename T>
	inline DynamicArray<T>::DynamicArray(size_t length)
	{
		m_pData = nullptr;
		m_size = length;
		allocate(m_size + ALLOCATE_STEPS);
		for (size_t i = 0; i < m_size; i++)
		{
			m_pData[i] = T();
		}
	}

	template<typename T>
	inline DynamicArray<T>::DynamicArray(const std::initializer_list<T>& il)
	{
		operator=(il);
	}

	template<typename T>
	inline DynamicArray<T>::~DynamicArray()
	{
		destroy();
	}

	template<typename T>
	inline void DynamicArray<T>::operator=(const DynamicArray<T>& d)
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

	template<typename T>
	inline void DynamicArray<T>::operator=(const std::initializer_list<T>& il)
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

	template<typename T>
	inline size_t DynamicArray<T>::size() const
	{
		return m_size;
	}

	template<typename T>
	inline bool DynamicArray<T>::isEmpty() const
	{
		return m_size == 0;
	}

	template<typename T>
	inline T* DynamicArray<T>::raw()
	{
		return m_pData;
	}

	template<typename T>
	inline T& DynamicArray<T>::at(size_t index)
	{
		if (index >= m_size)throw IllegalIndexException("Illegal dynamic array index");
		return m_pData[index];
	}

	template<typename T>
	inline T& DynamicArray<T>::operator[](size_t index)
	{
		return at(index);
	}

	template<typename T>
	inline const T& DynamicArray<T>::at(size_t index) const
	{
		if (index >= m_size)throw IllegalIndexException("Illegal dynamic array index");
		return m_pData[index];
	}

	template<typename T>
	inline const T& DynamicArray<T>::operator[](size_t index) const
	{
		return at(index);
	}

	template<typename T>
	inline T& DynamicArray<T>::front()
	{
		return m_pData[0];
	}

	template<typename T>
	inline const T& DynamicArray<T>::front() const
	{
		return m_pData[0];
	}

	template<typename T>
	inline T & DynamicArray<T>::back()
	{
		return m_pData[m_size - 1];
	}

	template<typename T>
	inline const T & DynamicArray<T>::back() const
	{
		return m_pData[m_size - 1];
	}

	template<typename T>
	inline T* DynamicArray<T>::begin()
	{
		return &m_pData[0];
	}

	template<typename T>
	inline const T* DynamicArray<T>::begin() const
	{
		return &m_pData[0];
	}

	template<typename T>
	inline T* DynamicArray<T>::end()
	{
		return &m_pData[m_size - 1];
	}

	template<typename T>
	inline const T * DynamicArray<T>::end() const
	{
		return &m_pData[m_size - 1];
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::resize(size_t newSize)
	{
		growIfNeeded(newSize);
		m_size = newSize;
		return *this;
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::push(const T& element)
	{
		growIfNeeded(1);
		m_pData[m_size] = element;
		m_size++;

		return *this;
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::push(const T&& element)
	{
		return push(element);
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::pop()
	{
		m_size--;
		return *this;
	}

	template<typename T>
	inline DynamicArray<T>& DynamicArray<T>::clear()
	{
		destroy();
		allocate(ALLOCATE_STEPS);

		return *this;
	}

	
	template<typename T>
	inline void DynamicArray<T>::destroy()
	{
		delete[] m_pData;
		m_pData = nullptr;
		m_size = 0;
		m_capacity = 0;
	}

	template<typename T>
	inline void DynamicArray<T>::growIfNeeded(size_t addedElemsCount)
	{
		size_t capReq = addedElemsCount + m_size;
		if (capReq < m_capacity)return;
		size_t allocSize = capReq - m_capacity;
		if (allocSize < ALLOCATE_STEPS)allocSize = ALLOCATE_STEPS;
		allocate(allocSize);
	}

	template<typename T>
	inline void DynamicArray<T>::allocate(size_t newAllocationsCount)
	{
		size_t newCap = m_capacity + newAllocationsCount;
		auto* pNewData = new T[newCap];

		if (m_capacity != 0)
		{
			memcpy(pNewData, m_pData, sizeof(T) * m_size);
		}

		m_capacity = newCap;
		m_pData = pNewData;
	}

}