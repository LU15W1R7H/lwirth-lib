#pragma once

#include "Standard.hpp"

#include "Shape.hpp"


namespace lw
{
	class LWAPI Circle : public Shape
	{
	public:
		Circle(f32 _radius, Vec2 _pos);
		~Circle();
		Circle* clone() const override;

		f32 m_radius = 0;

		f32 area() const override;
		void rotate(f32 angle, Vec2& center) override;
		
	};
}



