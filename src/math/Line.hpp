#pragma once

#include "../Standard.hpp"

#include "LinearAlgebra.hpp"

namespace lw
{
	struct Intersection
	{
	private:
		bool m_viable;
		Vec3 m_point;
	public:
		Intersection();
		Intersection(bool viable, Vec3 point);
		~Intersection() = default;

		bool viable();
		Vec3 get();
	};

	class Line
	{
		//is of form: OX = OP + t * OD
	public:
		enum Relationship { identical, parallel, intersecting, skew };

	private:
		Vec3 m_point;
		Vec3 m_direction; //should always be normalized
	public:
		Line() = default;
		Line(Vec3&& point, Vec3&& direction);
		Line(const Vec3& point, const Vec3& direction);
		~Line() = default;

		const Vec3& getPoint() const noexcept;
		const Vec3& getDirection() const noexcept;
		Vec3 getPoint(f32 parameter) const noexcept;

		void setPoint(const Vec3& point) noexcept;
		void setPoint(Vec3&& point) noexcept;
		void setDirection(const Vec3& direction) noexcept;
		void setDirection(Vec3&& direction) noexcept;
	};

	Line LineByPoints(const Vec3& p1, const Vec3& p2) noexcept;
	Line LineByPoints(Vec3&& p1, Vec3&& p2) noexcept;
	Line LineByPointAndDirection(const Vec3& point, const Vec3& direction) noexcept;
	Line LineByPointAndDirection(Vec3&& point, Vec3&& direction) noexcept;
	
	bool contains(const Line& line, const Vec3& point);
	bool contains(const Line& line, Vec3&& point);
	f32 angle(const Line& l1, const Line& l2);
	Intersection intersect(const Line& l1, const Line& l2);
	Vec3 nearestPointOnLine(const Line& line, const Vec3& point);
	Vec3 nearestPointOnLine(const Line& line, Vec3&& point);
	Line::Relationship relationship(const Line& l1, const Line& l2);

}

