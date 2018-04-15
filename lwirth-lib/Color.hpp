#pragma once

#include "Standard.hpp"

namespace lw
{
	class API Color
	{
	public:
		Color(F32 r, F32 g, F32 b, F32 a = 1.f);
		Color(U32 color);
		Color();
		~Color() = default;

		F32 r;
		F32 g;
		F32 b;
		F32 a;

		static Color red;
		static Color green;
		static Color blue;
		static Color black;
		static Color white;
	};
}