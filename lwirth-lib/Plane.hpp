#pragma once

#include "Standard.hpp"

#include "LinearAlgebra.hpp"
#include "Line.hpp"

namespace lw
{
	class LWAPI Plane
	{
		//is of form: ax + by + cz + d = 0;
	private:
		Vec3 m_normal; //should always be normalized
		f32 m_D;
	public:
		Plane() = default;
		Plane(Vec3&& normal, f32 d);
		Plane(const Vec3& normal, f32 d);
		~Plane() = default;

		void setNormal(Vec3&& normal);
		void setNormal(const Vec3& normal);
		void setD(f32 d);
		void set(Vec3&& normal, f32 d);
		void set(const Vec3& normal, f32 d);
		const Vec3& getNormal() const noexcept;
		const f32& getD() const noexcept;
	};

	LWAPI Plane PlaneByNormalAndD(const Vec3& normal, f32 d);
	LWAPI Plane PlaneByNormalAndD(Vec3&& normal, f32 d);
	LWAPI Plane PlaneByPoints(const Vec3& p1, const Vec3& p2, const Vec3& p3);
	LWAPI Plane PlaneByPoints(Vec3&& p1, Vec3&& p2, Vec3&& p3);
	LWAPI Plane PlaneByPointAndVectors(const Vec3& point, const Vec3& v1, const Vec3& v2);
	LWAPI Plane PlaneByPointAndVectors(Vec3&& point, Vec3&& v1, Vec3&& v2);
	LWAPI Plane PlaneByPointAndNormal(const Vec3& point, const Vec3& normal);
	LWAPI Plane PlaneByPointAndNormal(Vec3&& point, Vec3&& normal);
	LWAPI Plane PlaneEquidistant(const Vec3& p1, const Vec3& p2);
	LWAPI Plane PlaneEquidistant(Vec3&& p1, Vec3&& p2);
	
	LWAPI bool contains(const Plane& plane, const Vec3& point);
	LWAPI bool contains(const Plane& plane, Vec3&& point);
	LWAPI f32 angle(const Plane& plane, const Line& line);
	LWAPI f32 angle(const Plane& p1, const Plane& p2);
	LWAPI Intersection intersect(const Plane& plane, const Line& line);
	LWAPI Vec3 nearestPointOnPlane(const Plane& plane, const Vec3& point);
	LWAPI Vec3 nearestPointOnPlane(const Plane& plane, Vec3&& point);
	LWAPI Line intersectionLine(const Plane& p1, const Plane& p2);
	LWAPI Vec3 reflect(const Plane& plane, const Vec3& point);
	LWAPI Vec3 reflect(const Plane& plane, Vec3&& point);
	LWAPI Line reflect(const Plane& plane, const Line& line);


}


