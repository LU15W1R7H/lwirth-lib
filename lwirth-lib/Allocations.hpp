#pragma once

#include "Standard.hpp"

namespace lw
{
	namespace MEM
	{
		void* allocateUnaligned(size_t size_bytes) { return nullptr; }
		void freeUnaligned(void* pMem) {}

		void* allocateAligned(size_t size_bytes, size_t alignment);
		void freeAligned(void* pMem);
	}
}