#ifndef LWIRTH_STANDARD_HPP
#define LWIRTH_STANDARD_HPP

//library marcos
#define LW_NAME "lwirth-lib"
#define LW_VER_MAJOR 0
#define LW_VER_MINOR 0
#define LW_VER_PATCH 0

//compiler macros
#if defined(__clang__)
    #define LW_COMPILER_CLANG
#elif defined(__GNUG__)
    #define LW_COMPILER_GCC
#elif defined(_MSC_VER)
    #define LW_COMPILER_VS
#endif

//platform macros
#if defined(_WIN32)
    #define LW_PLATFORM_WINDOWS32
#elif
    #define LW_PLATFORM_WINDOWS64
#elif defined(__APPLE__)
    #define LW_PLATFORM_APPLE
#elif defined(__linux__)
    #define LW_PLATFORM_LINUX
#endif

//checks

//debug macros
#if defined(_DEBUG)
    #define LW_BUILD_DEBUG
#endif

#define LW_MAKE_VERSION(major, minor, patch) \
    (((major) << 22) | ((minor) << 12) | (patch))

#if defined(LW_BUILD_DEBUG)
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

#include <cstdint>
#include <cstddef>

using size_t = std::size_t;
using u8	= std::uint8_t;
using u16	= std::uint16_t;
using u32	= std::uint32_t;
using u64	= std::uint64_t;
using i8	= std::int8_t;
using i16	= std::int16_t;
using i32	= std::int32_t;
using i64	= std::int64_t;
using f32	= float;
using f64	= double;
using f128	= long double;
using byte	= u8;


//#TODO: better debugbreak()? (maybe __buildin_trap() for GCC and clang)
namespace lw
{
#if defined(LW_COMPILER_CLANG)
    inline void debugBreak()
     {
        asm("int $3");
     }
#elif defined(LW_COMPILER_GCC)
    inline void debugBreak() __attribute__((always_inline));

    inline void debugBreak()
    {
        asm("int $3");
    }
#elif defined(LW_COMPILER_VS)
    __inline void debugBreak()
    {
        __debugbreak();
    }
#endif
    
    
}

#endif //LWIRTH_STANDARD_HPP