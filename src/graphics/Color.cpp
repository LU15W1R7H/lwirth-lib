#include "Color.hpp"

namespace lw
{
	Color::Color(f32 r, f32 g, f32 b, f32 a)
		: r(r), g(g), b(b), a(a)
	{
	}

	Color::Color(u32 color)
	{
		u8 red = (color & 0xFF000000) >> 24;
		u8 green = (color & 0x00FF0000) >> 16;
		u8 blue = (color & 0x0000FF00) >> 8;

		r = red / 255.f;
		g = green / 255.f;
		b = blue / 255.f;
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