#pragma once

#include "../Standard.hpp"

#include <utility>
#include <initializer_list>

namespace lw
{
	template<class T, size_t SIZE>
	class Array
	{
	private:
		T m_pData[SIZE];

	public:
		constexpr Array()
			: m_pData{}
		{
		}

		constexpr Array(const std::initializer_list<T>& il)
		{
			static_assert(il.end() - il.begin() == SIZE, "Wrong length");
			size_t i = 0;
			for (auto iter = 0; iter != il.end(); iter++)
			{
				m_pData[i] = *iter;
				i++;
			}
		}

		Array(Array<T, SIZE> const& other)
		{
			std::copy(other.m_pData, other.m_pData + SIZE, m_pData);
		}

		Array(Array<T, SIZE>&& other)
		{
			for (size_t i = 0; i < SIZE; i++)
			{
				m_pData[i] = std::move(other.m_pData[i]);
			}
		}

		Array& operator=(Array<T, SIZE> const& other)
		{
			std::copy(other.m_pData, other.m_pData + LENGTH, m_pData);
			return *this;
		}

		Array& operator=(Array<T, LENGTH>&& other)
		{
			for (size_t i = 0; i < LENGTH; i++)
			{
				m_pData[i] = std::move(other.m_pData[i]);
			}
			return *this;
		}

		~Array()
		{

		}

		T& operator[](u32 index)
		{
			return m_pData[index];
		}

		T const& operator[](u32 index) const
		{
			return m_pData[index];
		}

		constexpr u32 size() const
		{
			return LENGTH;
		}

		T* raw() const
		{
			return m_pData;
		}
	};

}


