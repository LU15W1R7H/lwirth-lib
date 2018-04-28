#pragma once

#define LWIRTH_NAME "lwirth-lib"
#define LWIRTH_VERSION_MAJOR 0
#define LWIRTH_VERSION_MINOR 0
#define LWIRTH_VERSION_PATCH 0


#ifdef COMPILE_LWIRTH
#define API __declspec(dllexport)
#else
#define API __declspec(dllimport)
#endif

#ifdef _DEBUG

 //#TODO inform of error (#expr, __FILE__, __LINE__)
#define ASSERT(expr) \
	if(expr) {} \
	else \
	{ \
		__debugBreak; \
	}
#else
#define ASSERT(expr)
#endif

#include <stdint.h>

using u8	= uint8_t;
using u16	= uint16_t;
using u32	= uint32_t;
using u64	= uint64_t;
using i8	= int8_t;
using i16	= int16_t;
using i32	= int32_t;
using i64	= int64_t;
using f32	= float;
using f64	= double;
using f128	= long double;
using byte	= unsigned char;



namespace lw
{
	inline void debugBreak()
	{
		__debugbreak();
	}
	
	
}

