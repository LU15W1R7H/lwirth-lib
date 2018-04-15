#pragma once

#include "Standard.hpp"

#include <xmmintrin.h>

namespace lw
{
#define SHUFFLE_PARAM(x, y, z, w) \
	((x) | ((y) << 2) | ((z) << 4) | ((w) << 6))

#define _mm_replicate_x_ps(v) \
	_mm_shuffle_ps(v, v, SHUFFLE_PARAM(0, 0, 0, 0))

#define _mm_replicate_y_ps(v) \
	_mm_shuffle_ps(v, v, SHUFFLE_PARAM(1, 1, 1, 1))

#define _mm_replicate_z_ps(v) \
	_mm_shuffle_ps(v, v, SHUFFLE_PARAM(2, 2, 2, 2))

#define _mm_replicate_w_ps(v) \
	_mm_shuffle_ps(v, v, SHUFFLE_PARAM(3, 3, 3, 3))

#define _mm_madd_ps(a, b, c) \
	_mm_add_ps(_mm_mul_ps(a, b), c)

	__m128 mulVectorMatrix(const __m128& v, const __m128 Mrows[4])
	{
		__m128 result;

		result = _mm_mul_ps(_mm_replicate_x_ps(v), Mrows[0]);
		result = _mm_madd_ps(_mm_replicate_y_ps(v), Mrows[1], result);
		result = _mm_madd_ps(_mm_replicate_z_ps(v), Mrows[2], result);
		result = _mm_madd_ps(_mm_replicate_w_ps(v), Mrows[3], result);
		
		return result;
	}

	class SIMDvector
	{
	public:
		__m128 m_register;

	public:
		SIMDvector(f32 x, f32 y, f32 z, f32 w);

		void set(f32 x, f32 y, f32 z, f32 w);

		SIMDvector& operator+=(const SIMDvector& other);
		SIMDvector& operator-=(const SIMDvector& other);
		SIMDvector& operator*=(const SIMDvector& other);

		/*friend extern bool operator==(Vec3 const& v1, Vec3 const& v2);
		friend extern bool operator!=(Vec3 const& v1, Vec3 const& v2);
		friend extern Vec3 operator-(Vec3 v);
		friend extern Vec3 operator+(Vec3 v1, Vec3 const& v2);
		friend extern Vec3 operator-(Vec3 v1, Vec3 const& v2);
		friend extern F32 operator*(Vec3 const& v1, Vec3 const& v2);
		friend extern Vec3 operator^(Vec3 const& v1, Vec3 const& v2);
		friend extern Vec3 operator*(Vec3 v, F32 scalar);
		friend extern Vec3 operator*(F32 scalar, Vec3 v);
		friend extern Vec3 operator/(Vec3 v, F32 scalar);
		friend extern Vec3 min(Vec3 const& v1, Vec3 const& v2);
		friend extern Vec3 max(Vec3 const& v1, Vec3 const& v2);*/
	};

	class SIMDmatrix
	{
	public:
		__m128 m_registers[4];

	public:
		SIMDmatrix(f32 x, f32 y, f32 z, f32 w);

		void set(f32 x, f32 y, f32 z, f32 w);

		SIMDmatrix& operator+=(const SIMDvector& other);
		SIMDmatrix& operator-=(const SIMDvector& other);
		SIMDmatrix& operator*=(const SIMDvector& other);

		
	};
	
		
}