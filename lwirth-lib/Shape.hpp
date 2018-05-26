#pragma once

#include "Standard.hpp"

#include "List.hpp"
#include "Vector.hpp"

namespace lw
{
	class LWAPI Shape
	{
	public:
		Shape(Vec2 _pos);
		virtual Shape* clone() const = 0;

		Vec2 m_pos;

		virtual f32 area() const = 0;
		virtual void rotate(f32 angle, Vec2& center) = 0;
	};

	LWAPI bool intersects(Shape& s1, Shape& s2);
}