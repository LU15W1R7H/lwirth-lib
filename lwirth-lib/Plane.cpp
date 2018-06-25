#include "stdafx.hpp"
#include "Plane.hpp"


namespace lw
{

	Plane::Plane(Vec3&& normal, f32 d)
		: m_normal(normal), m_D(d)
	{
		ASSERT(normal.magSqd() < 1.01f && normal.magSqd() > 0.99f, "Normal must be normalized");
	}

	Plane::Plane(const Vec3 & normal, f32 d)
		: m_normal(normal), m_D(d)
	{
		ASSERT(normal.magSqd() < 1.01f && normal.magSqd() > 0.99f, "Normal must be normalized");
	}

	void Plane::setNormal(Vec3&& normal)
	{
		m_normal = norm(normal);
	}

	void Plane::setNormal(const Vec3 & normal)
	{
		m_normal = norm(normal);
	}

	void Plane::setD(f32 d)
	{
		m_D = d;
	}

	void Plane::set(Vec3 && normal, f32 d)
	{
		m_normal = norm(normal);
		m_D = d;
	}

	void Plane::set(const Vec3 & normal, f32 d)
	{
		m_normal = norm(normal);
		m_D = d;
	}

	const Vec3& Plane::getNormal() const noexcept
	{
		return m_normal;
	}

	const f32& Plane::getD() const noexcept
	{
		return m_D;
	}

	Plane PlaneByNormalAndD(const Vec3& normal, f32 d)
	{
		Vec3 point(0.f, 0.f, -d / normal.z);
		return PlaneByPointAndNormal(point, normal);
	}

	Plane PlaneByNormalAndD(Vec3&& normal, f32 d)
	{
		Vec3 point(0, 0, -d / normal.z);
		return PlaneByPointAndNormal(point, normal);
	}

	Plane PlaneByPoints(const Vec3& p1, const Vec3& p2, const Vec3& p3)
	{
		return PlaneByPointAndVectors(p1, VectorByPoints(p1, p2), VectorByPoints(p1, p3));
	}

	Plane PlaneByPoints(Vec3&& p1, Vec3&& p2, Vec3&& p3)
	{
		return PlaneByPointAndVectors(p1, VectorByPoints(p1, p2), VectorByPoints(p1, p3));
	}

	Plane PlaneByPointAndVectors(const Vec3& point, const Vec3& v1, const Vec3& v2)
	{
		return PlaneByPointAndNormal(point, cross(v1, v2));
	}

	Plane PlaneByPointAndVectors(Vec3&& point, Vec3&& v1, Vec3&& v2)
	{
		return PlaneByPointAndNormal(point, cross(v1, v2));
	}

	Plane PlaneByPointAndNormal(const Vec3& point, const Vec3& normal)
	{
		Vec3 v = norm(normal);
		return Plane(v, -dot(point, v));
	}

	Plane PlaneByPointAndNormal(Vec3&& point, Vec3&& normal)
	{
		Vec3 v = norm(normal);
		return Plane(v, -dot(point, v));
	}

	Plane PlaneEquidistant(const Vec3& p1, const Vec3& p2)
	{
		auto v = VectorByPoints(p1, p2);
		return PlaneByPointAndNormal(p1 + v / 2, v);
	}

	Plane PlaneEquidistant(Vec3&& p1, Vec3&& p2)
	{
		auto v = VectorByPoints(p1, p2);
		return PlaneByPointAndNormal(p1 + v / 2, v);
	}

	bool contains(const Plane& plane, const Vec3& point)
	{
		return dot(plane.getNormal(), point) + plane.getD() == 0;
	}

	bool contains(const Plane& plane, Vec3&& point)
	{
		f32 result = dot(plane.getNormal(), point) + plane.getD();
		return result < 0.01 && result > -0.01;
	}

	f32 angle(const Plane & plane, const Line & line)
	{
		return PI / 2 * angle(plane.getNormal(), line.getDirection());
	}

	f32 angle(const Plane& p1, const Plane& p2)
	{
		return angle(p1.getNormal(), p2.getNormal());
	}

	Intersection intersect(const Plane& plane, const Line& line)
	{
		f32 dotDirection = dot(plane.getNormal(), line.getDirection());
		if (dotDirection == 0)
		{
			return Intersection(false, {});
		}
		else
		{
			f32 parameter = (-dot(plane.getNormal(), line.getPoint()) - plane.getD()) / dotDirection;
			return Intersection(true, line.getPoint(parameter));
		}
	}

	Vec3 nearestPointOnPlane(const Plane& plane, const Vec3& point)
	{
		return intersect(plane, LineByPointAndDirection(point, plane.getNormal())).get();
	}

	Vec3 nearestPointOnPlane(const Plane& plane, Vec3&& point)
	{
		return intersect(plane, LineByPointAndDirection(point, plane.getNormal())).get();
	}

	Line intersectionLine(const Plane& p1, const Plane& p2)
	{
		auto& n1 = p1.getNormal();
		auto& n2 = p2.getNormal();
		f32 denominator = n1.y * n2.x - n1.x * n2.y;
		return LineByPointAndDirection(
			Vec3(
				(p1.getD() * n2.y - n1.y * p2.getD()) / denominator,
				(p1.getD() * n2.x - n1.x * p2.getD()) / -denominator,
				0), 
			cross(n1, n2));
	}

	Vec3 reflect(const Plane& plane, const Vec3& point)
	{
		Line line = LineByPointAndDirection(point, plane.getNormal()); //create line for intersection
		return line.getPoint(
			(-dot(plane.getNormal(), line.getPoint()) - plane.getD()) / dot(plane.getNormal(), line.getDirection()) //parameter for nearestPoint
			* 2);
	}

	Vec3 reflect(const Plane & plane, Vec3 && point)
	{
		Line line = LineByPointAndDirection(point, plane.getNormal()); //create line for intersection
		return line.getPoint(
			(-dot(plane.getNormal(), line.getPoint()) - plane.getD()) / dot(plane.getNormal(), line.getDirection()) //parameter for nearestPoint
			* 2);
	}

	Line reflect(const Plane& plane, const Line& line)
	{
		Vec3 inter = intersect(plane, line).get();
		Vec3 refPoint = reflect(plane, line.getPoint());
		return LineByPoints(inter, refPoint);
	}

}
