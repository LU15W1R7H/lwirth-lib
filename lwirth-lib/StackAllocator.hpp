#pragma once

#include "Standard.hpp"

namespace lw
{
	//double-ended stackallocator?
	class API StackAllocator
	{
	public:
		typedef u32 Marker;

		explicit StackAllocator(u32 stackSize_bytes) {}
		
		void* alloc(u32 size_bytes) { return nullptr; }

		Marker getMarker() { return 0; } //Maker to current stack top

		void freeToMarker(Marker marker) {} //roll stack back until marker

		void clear() {} //roll stack back to zero
	};

}


