#pragma once

#define LW_NAME "lwirth-lib"
#define LW_VER_MAJOR 0
#define LW_VER_MINOR 0
#define LW_VER_PATCH 0


#ifdef LW_DLL

#ifdef LW_COMPILING
#define LWAPI __declspec(dllexport)
#else
#define LWAPI __declspec(dllimport)
#endif //!LW_COMPILING

#else //!LW_DLL
#define LWAPI
#endif //!else


#define DISABLE_COPYING(T)\
	T(const T&) = delete; \
	T& operator=(const T&) = delete;

#define DISABLE_MOVING(T)\
	T(T&&) = delete; \
	T& operator=(T&&) = delete;

#define DISABLE_COP_MOV(T)\
	DISABLE_COPYING(T); \
	DISABLE_MOVING(T);

#define LW_MAKE_VERSION(major, minor, patch) \
	(((major) << 22) | ((minor) << 12) | (patch))

#ifdef _DEBUG

 //#TODO inform of error (#expr, __FILE__, __LINE__)
#define ASSERT(expr) \
	if(expr) {} \
	else \
	{ \
		__debugbreak(); \
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

