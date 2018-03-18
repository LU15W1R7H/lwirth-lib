#pragma once

#include "Standard.hpp"

#include <initializer_list>

namespace lw
{
	/*
	template<typename T>
	class Chain
	{
	private:
		T* m_pData;
		U32 m_length;
		U32 m_capacity;

	public:
		Chain();
		Chain(U32 length);
		Chain(T... elems);
		Chain(const std::initializer_list<T>& il);
		
		U32 length() const;
		T& at(U32 index) const;
		T& operator[](U32 index) const;
		List& push(T... elems);

		T* raw();

	private:
		void growIfNeeded(U32 addedElementsCount);
	};
	*/
}