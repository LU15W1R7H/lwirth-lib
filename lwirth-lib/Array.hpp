#pragma once

#include "Standard.hpp"

#include <utility>
#include <initializer_list>

namespace lw
{
	template<typename T, u32 LENGTH>
	class Array
	{
	private:
		T m_pdata[LENGTH];

	public:
		Array()
		{

		}

		Array(std::initializer_list<T> const& il)
		{
			static_assert(il.end() - il.begin() == LENGTH, "Wrong length");
			size_t i = 0;
			for (auto iter = 0; iter != il.end(); iter++)
			{
				m_pdata = *iter;
				i++;
			}
		}

		Array(Array<T, LENGTH> const& other)
		{
			std::copy(other.m_pdata, other.m_pdata + LENGTH, m_pdata);
		}

		Array(Array<T, LENGTH>&& other)
		{
			for (size_t i = 0; i < LENGTH; i++)
			{
				m_pdata[i] = std::move(other.m_pdata[i]);
			}
		}

		Array& operator=(Array<T, LENGTH> const& other)
		{
			std::copy(other.m_pdata, other.m_pdata + LENGTH, m_pdata);
			return *this;
		}

		Array& operator=(Array<T, LENGTH>&& other)
		{
			for (size_t i = 0; i < LENGTH; i++)
			{
				m_pdata[i] = std::move(other.m_pdata[i]);
			}
			return *this;
		}

		~Array()
		{

		}

		T& operator[](u32 index)
		{
			return m_pdata[index];
		}

		T const& operator[](u32 index) const
		{
			return m_pdata[index];
		}

		constexpr u32 size() const
		{
			return LENGTH;
		}

		T* raw() const
		{
			return m_pdata;
		}
	};

}


