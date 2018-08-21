#pragma once

#include "../Standard.hpp"

#include "../memory/List.hpp"
#include "Vector.hpp"

namespace lw
{
	class Shape
	{
	public:
		Shape(Vec2 _pos);
		virtual Shape* clone() const = 0;

		Vec2 m_pos;

		virtual f32 area() const = 0;
		virtual void rotate(f32 angle, Vec2& center) = 0;
	};

	bool intersects(Shape& s1, Shape& s2);
}