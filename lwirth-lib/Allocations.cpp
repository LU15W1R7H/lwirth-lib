#include "stdafx.hpp"

#include "Allocations.hpp"

namespace lw
{
	namespace MEM
	{
		void* allocateAligned(Size size_bytes, Size alignment)
		{
			//ASSERT(alignment >= 1);
			//ASSERT(alignment <= 128);
			//ASSERT(alignment & (alignment - 1) == 0); //check if power 2

			Size expandedSize = size_bytes + alignment;

			uintptr_t rawAddress = reinterpret_cast<uintptr_t>(allocateUnaligned(expandedSize));

			Size mask = alignment - 1;
			uintptr_t misalignment = rawAddress & mask;
			ptrdiff_t adjustment = alignment - misalignment;

			uintptr_t alignedAddress = rawAddress + adjustment;

			//ASSERT(adjustment < 256);
			U8* pAlignedMem = reinterpret_cast<U8*>(alignedAddress);
			pAlignedMem[-1] = static_cast<U8>(adjustment);

			return static_cast<void*>(pAlignedMem);
		}

		void freeAligned(void * pMem)
		{
			const U8* pAlignedMem = reinterpret_cast<const U8*>(pMem);

			uintptr_t alignedAddress = reinterpret_cast<uintptr_t>(pMem);
			ptrdiff_t adjustment = static_cast<ptrdiff_t>(pAlignedMem[-1]);

			uintptr_t rawAddress = alignedAddress - adjustment;
			void* pRawMem = reinterpret_cast<void*>(rawAddress);

			freeUnaligned(pRawMem);
		}
	}
}