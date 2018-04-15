#pragma once

#include "Standard.hpp"

#include "DynamicArray.hpp"
#include "Vector.hpp"

namespace lw
{
	class API Shape
	{
	public:
		Shape(Vec2 _pos);
		virtual Shape* clone() const = 0;

		Vec2 m_pos;

		virtual f32 area() const = 0;
		virtual void rotate(f32 angle, Vec2& center) = 0;
	};

	API bool intersects(Shape& s1, Shape& s2);
}