#pragma once

#include "Standard.hpp"

#include "Shape.hpp"


namespace lw
{
	class API Circle : public Shape
	{
	public:
		Circle(F32 _radius, Vec2 _pos);
		~Circle();
		Circle* clone() const override;

		F32 m_radius = 0;

		F32 area() const override;
		void rotate(F32 angle, Vec2& center) override;
		
	};
}



