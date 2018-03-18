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

 //TODO inform of error (#expr, __FILE__, __LINE__)
#define ASSERT(expr) \
	if(expr) {} \
	else \
	{ \
		__debugBreak; \
	}
#else
#define ASSERT(expr)
#endif

#include <cstdint>

typedef std::uint8_t	U8;
typedef std::uint16_t	U16;
typedef std::uint32_t	U32;
typedef std::uint64_t	U64;
typedef std::int8_t		I8;
typedef std::int16_t	I16;
typedef std::int32_t	I32;
typedef std::int64_t	I64;
typedef float			F32;
typedef double			D64;
typedef char			Byte;
typedef size_t			Size;


typedef const char* CString;

namespace lw
{
	
}

