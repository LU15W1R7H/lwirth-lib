#pragma once

#include "Standard.hpp"
#include "Math.hpp"

namespace lw
{
	class LWAPI VectorField2D
	{
	private:
		f32 m_width;
		f32 m_height;
		f32 m_precision;
		
		size_t m_xAmount;
		size_t m_yAmount;
		Vec2** m_vectors;

	public:
		VectorField2D(f32 width, f32 height, f32 precision);

		Vec2 get(f32 x, f32 y);
		f32 getStrength(f32 x, f32 y);
		void set(f32 x, f32 y, const Vec2& v);
		void set(f32 x, f32 y, Vec2&& v);
		void setStrength(f32 x, f32 y, f32 strength);
	};
}



