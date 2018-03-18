#pragma once

#include "Standard.hpp"

#include <vector>
#include "Vector2.hpp"

namespace lw
{
	class API Shape
	{
	public:
		Shape(Vector2 _pos);
		virtual Shape* clone() const = 0;

		Vector2 m_pos;

		virtual F32 area() const = 0;
		virtual void rotate(F32 angle, Vector2& center) = 0;
	};

	API bool intersects(Shape& s1, Shape& s2);
}