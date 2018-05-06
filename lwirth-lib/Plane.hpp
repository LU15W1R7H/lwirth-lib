#pragma once

#include "Standard.hpp"

#include "Math.hpp"
#include "Line.hpp"

namespace lw
{
	

	class API Plane
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

	API Plane PlaneByNormalAndD(const Vec3& normal, f32 d);
	API Plane PlaneByNormalAndD(Vec3&& normal, f32 d);
	API Plane PlaneByPoints(const Vec3& p1, const Vec3& p2, const Vec3& p3);
	API Plane PlaneByPoints(Vec3&& p1, Vec3&& p2, Vec3&& p3);
	API Plane PlaneByPointAndVectors(const Vec3& point, const Vec3& v1, const Vec3& v2);
	API Plane PlaneByPointAndVectors(Vec3&& point, Vec3&& v1, Vec3&& v2);
	API Plane PlaneByPointAndNormal(const Vec3& point, const Vec3& normal);
	API Plane PlaneByPointAndNormal(Vec3&& point, Vec3&& normal);
	API Plane PlaneEquidistant(const Vec3& p1, const Vec3& p2);
	API Plane PlaneEquidistant(Vec3&& p1, Vec3&& p2);
	
	API bool contains(const Plane& plane, const Vec3& point);
	API bool contains(const Plane& plane, Vec3&& point);
	API f32 angle(const Plane& plane, const Line& line);
	API f32 angle(const Plane& p1, const Plane& p2);
	API Intersection intersect(const Plane& plane, const Line& line);
	API Vec3 nearestPointOnPlane(const Plane& plane, const Vec3& point);
	API Vec3 nearestPointOnPlane(const Plane& plane, Vec3&& point);
	API Line intersectionLine(const Plane& p1, const Plane& p2);
	API Vec3 reflect(const Plane& plane, const Vec3& point);
	API Vec3 reflect(const Plane& plane, Vec3&& point);
	API Line reflect(const Plane& plane, const Line& line);


}


