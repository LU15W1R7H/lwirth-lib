#pragma once

#include "Standard.hpp"

namespace lw
{
	class LWAPI Color
	{
	public:
		Color(f32 r, f32 g, f32 b, f32 a = 1.f);
		Color(u32 color);
		Color();
		~Color() = default;

		f32 r;
		f32 g;
		f32 b;
		f32 a;

		static Color red;
		static Color green;
		static Color blue;
		static Color black;
		static Color white;
	};
}