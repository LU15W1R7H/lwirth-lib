#include "stdafx.hpp"
#include "Color.hpp"

namespace lw
{
	Color::Color(F32 r, F32 g, F32 b, F32 a)
		: r(r), g(g), b(b), a(a)
	{
	}

	Color::Color()
		: r(0.f), g(0.f), b(0.f), a(0.f)
	{
	}

	Color Color::red(1.f, 0.f, 0.f, 1.f);
	Color Color::green(0.f, 1.f, 0.f, 1.f);
	Color Color::blue(0.f, 0.f, 1.f, 1.f);
	Color Color::black(0.f, 0.f, 0.f, 1.f);
	Color Color::white(1.f, 1.f, 1.f, 1.f);
}