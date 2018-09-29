#ifndef LWIRTH_STANDARD_HPP
#define LWIRTH_STANDARD_HPP

#include <iostream>

#define LW_NAME "lwirth-lib"
#define LW_VER_MAJOR 0
#define LW_VER_MINOR 0
#define LW_VER_PATCH 0


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

#define ASSERT(expr, msg) \
	if(expr) {} \
	else \
	{ \
		std::cerr << "ASSERT: " << msg << " (file: " << __FILE__ << " line: " << __LINE__ << ")\n"; \
		__debugbreak(); \
	}
#else
#define ASSERT(expr, msg)
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
#if defined(__clang__)
	asm("int $3");
#elif defined(__GNUC__) || defined(__GNUG__)
	inline void debugBreak() __attribute__((always_inline));

	inline void debugBreak()
	{
		asm("0:"
			".pushsection embed-breakpoints;"
			".quad 0b;"
			".popsection;");
	}
#elif defined(_MSC_VER)
	__inline inline void debugBreak()
	{
		__debugbreak();
	}
#endif
	
	
}

#endif //LWIRTH_STANDARD_HPP