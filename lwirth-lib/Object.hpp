#pragma once

#include "Standard.hpp"

#include <memory>

#include "Shape.hpp"
#include "SimulationTime.hpp"

namespace lw
{
	namespace phy
	{
		class API Object
		{
		public:
			Object() = delete;
			Object(Shape& _shape, F32 _mass);
			~Object();

			Vec2& m_pos;
			Vec2 m_vel;
			Vec2 m_acc;
			F32 m_mass;

			void applyForce(Vec2 force);

			virtual void update(const Duration& duration);

			const Shape& getShape();

		private:
			std::shared_ptr<Shape> m_shape;

			static friend bool intersects(Object& o1, Object& o2);
		};

		
	}
	
}



