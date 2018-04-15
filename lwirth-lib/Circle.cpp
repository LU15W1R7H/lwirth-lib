#include "stdafx.hpp"
#include "Circle.hpp"

#include "Utils.hpp"

namespace lw
{

	Circle::Circle(f32 _radius, Vec2 _pos)
		: Shape(_pos), m_radius(_radius)
	{
	}

	Circle::~Circle()
	{
	}

	Circle* Circle::clone() const
	{
		return new Circle(*this);
	}

	f32 Circle::area() const
	{
		return PI * m_radius * m_radius;
	}

	
	void Circle::rotate(f32 angle, Vec2& center)
	{
		//do nothing
	}
}


