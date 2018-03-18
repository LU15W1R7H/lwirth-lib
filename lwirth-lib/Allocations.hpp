#pragma once

#include "Standard.hpp"

namespace lw
{
	namespace MEM
	{
		void* allocateUnaligned(Size size_bytes) { return nullptr; }
		void freeUnaligned(void* pMem) {}

		void* allocateAligned(Size size_bytes, Size alignment);
		void freeAligned(void* pMem);
	}
}