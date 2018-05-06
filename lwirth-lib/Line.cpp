#include "stdafx.hpp"
#include "Line.hpp"

#include "Plane.hpp"

namespace lw
{
	Intersection::Intersection()
		: m_viable(false), m_point()
	{
	}

	Intersection::Intersection(bool viable, Vec3 point)
		: m_viable(viable), m_point(point)
	{
	}

	bool Intersection::viable()
	{
		return m_viable;
	}

	Vec3 Intersection::get()
	{
		if (!m_viable)throw std::runtime_error("Intersection is not viable");
		return m_point;
	}


	Line::Line(Vec3&& point, Vec3&& direction)
		: m_point(point), m_direction(norm(direction))
	{
	}

	Line::Line(const Vec3& point, const Vec3& direction)
		: m_point(point), m_direction(norm(direction))
	{
	}

	const Vec3& Line::getPoint() const noexcept
	{
		return m_point;
	}

	Vec3 Line::getPoint(f32 parameter) const noexcept
	{
		return m_point + parameter * m_direction;
	}

	const Vec3& Line::getDirection() const noexcept
	{
		return m_direction;
	}

	void Line::setPoint(const Vec3& point) noexcept
	{
		m_point = point;
	}

	void Line::setPoint(Vec3&& point) noexcept
	{
		m_point = point;
	}

	void Line::setDirection(const Vec3& direction) noexcept
	{
		m_direction = norm(direction);
	}

	void Line::setDirection(Vec3&& direction) noexcept
	{
		m_direction = norm(direction);
	}

	Line LineByPoints(const Vec3& p1, const Vec3& p2) noexcept
	{
		return Line(p1, VectorByPoints(p1, p2));
	}

	Line LineByPoints(Vec3&& p1, Vec3&& p2) noexcept
	{
		return Line(p1, VectorByPoints(p1, p2));
	}

	Line LineByPointAndDirection(const Vec3& point, const Vec3& direction) noexcept
	{
		return Line(point, direction);
	}

	Line LineByPointAndDirection(Vec3&& point, Vec3&& direction) noexcept
	{
		return Line(point, direction);
	}

	bool contains(const Line& line, const Vec3& point)
	{
		Vec3 p = line.getPoint();
		Vec3 d = line.getDirection();
		f32 parameter = (point.x - p.x) / d.x;
		return (p.y + parameter * d.y == point.y) && (p.z + parameter * d.z == point.z);
	}

	bool contains(const Line& line, Vec3&& point)
	{
		Vec3 p = line.getPoint();
		Vec3 d = line.getDirection();
		f32 parameter = (point.x - p.x) / d.x;
		return (p.y + parameter * d.y == point.y) && (p.z + parameter * d.z == point.z);
	}

	f32 angle(const Line & l1, const Line & l2)
	{
		return angle(l1.getDirection(), l2.getDirection());
	}

	Intersection intersect(const Line& l1, const Line& l2)
	{
		Vec3 p1 = l1.getPoint();
		Vec3 p2 = l2.getPoint();
		Vec3 d1 = l1.getDirection();
		Vec3 d2 = l2.getDirection();
		Vec3 pdiff = p1 - p2;
		f32 denominator = d2.x * d1.y - d1.x * d2.y;

		f32 para1 = (d2.y * pdiff.x + d2.x * -pdiff.y) / denominator;
		f32 para2 = (d1.y * pdiff.x + d1.x * -pdiff.y) / denominator;

		bool eqn3Agree = p1.z + para1 * d1.z == p2.z + para2 * d2.z;
		if (eqn3Agree)
		{
			return Intersection(true, l1.getPoint(para1));
		}
		else
		{
			return Intersection(false, {});
		}
	}

	Vec3 nearestPointOnLine(const Line& line, const Vec3& point)
	{
		Plane plane = PlaneByPointAndNormal(point, line.getDirection());
		return intersect(plane, line).get();
	}

	Vec3 nearestPointOnLine(const Line& line, Vec3&& point)
	{
		Plane plane = PlaneByPointAndNormal(point, line.getDirection());
		return intersect(plane, line).get();
	}

	Line::Relationship relationship(const Line& l1, const Line& l2)
	{
		if (collinear(l1.getDirection(), l2.getDirection()))
		{
			if (contains(l1, l2.getPoint()))
				return Line::identical;
			else
				return Line::parallel;
		}
		else
		{
			if (intersect(l1, l2).viable())
				return Line::intersecting; //#TODO return point
			else
				return Line::skew;
		}
	}
	
}