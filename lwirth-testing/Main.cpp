#include "lwirth.hpp"

#include "Plane.hpp"

using namespace lw;

int main()
{
	Line l1 = LineByPointAndDirection(Vec3(3, 3, 0), Vec3(0.5f, 1, 1));
	Line l2 = LineByPointAndDirection(Vec3(-1, 0, 0), Vec3(0.5f, 1, 0));
	auto inter = intersect(l1, l2);
	auto b = relationship(l1, l2);

	Line l3 = LineByPointAndDirection(Vec3(-3, -2, 1), Vec3(2, 2, 1));
	Vec3 nearest = nearestPointOnLine(l3, Vec3(2, 0, 5));

	Plane p1 = PlaneByNormalAndD(Vec3(2, -1, 2), -4);
	Plane p2 = PlaneByNormalAndD(Vec3(1, 1, -1), 1);
	Line interLine = intersectionLine(p1, p2);

	Plane p3 = PlaneByNormalAndD(Vec3(2, -3, 4), -78);
	Vec3 reflected = reflect(p3, Vec3(1, -2, 3));

	Plane p4 = PlaneByNormalAndD(Vec3(4, -3, -1), -24);
	Line l4 = LineByPoints(Vec3(-2, -4, 6), Vec3(2, -6, 2));
	Line reflectedLine = reflect(p4, l4);


	std::cin.get();
}