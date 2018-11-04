#include "Shape.hpp"

namespace lw
{
	Shape::Shape(Vec2 _pos)
		: m_pos(_pos)
	{
	}

	bool intersects(Shape& s1, Shape& s2)
	{
		return false;
	}
}