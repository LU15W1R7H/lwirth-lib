#include "stdafx.hpp"
#include "Shape.hpp"

namespace lw
{
	Shape::Shape(Vector2 _pos)
		: m_pos(m_pos)
	{
	}

	bool intersects(Shape& s1, Shape& s2)
	{
		return false;
	}
}