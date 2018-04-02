#include "stdafx.hpp"
#include "Circle.hpp"

#include "Utils.hpp"

namespace lw
{

	Circle::Circle(F32 _radius, Vec2 _pos)
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

	F32 Circle::area() const
	{
		return PI * m_radius * m_radius;
	}

	
	void Circle::rotate(F32 angle, Vec2& center)
	{
		//do nothing
	}
}


