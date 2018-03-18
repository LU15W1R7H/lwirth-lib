#include "stdafx.hpp"
#include "Object.hpp"

namespace lw
{
	namespace phy
	{
		
		Object::Object(Shape& _shape, F32 _mass)
			: m_shape(_shape.clone()), m_pos(m_shape->m_pos), m_mass(_mass)
		{
		}

		Object::~Object()
		{
		}

		void Object::applyForce(Vector2 force)
		{
			m_acc += force / m_mass;
		}

		void Object::update(const Duration& duration)
		{
			m_vel += m_acc * duration.span;
			m_acc *= 0;
			m_pos += m_vel * duration.span;
		}

		const Shape& Object::getShape()
		{
			return *m_shape;
		}


		bool intersects(Object& o1, Object& o2)
		{
			return lw::intersects(*o1.m_shape, *o2.m_shape);
		}
	}
}


