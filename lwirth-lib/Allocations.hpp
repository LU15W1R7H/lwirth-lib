#pragma once

#include "Standard.hpp"

namespace lw
{
	namespace MEM
	{
		void* allocateUnaligned(size_t size_bytes);
		void freeUnaligned(void* pMem);

		void* allocateAligned(size_t size_bytes, size_t alignment);
		void freeAligned(void* pMem);

		template<class T>
		void execDestructor(const void* data)
		{
			auto obj = static_cast<const T*>(data);
			obj->~T();
		}
	}
}