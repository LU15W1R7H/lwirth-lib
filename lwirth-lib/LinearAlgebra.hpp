#pragma once

#include "Vector.hpp"
#include "Vector2.hpp"
#include "Vector3.hpp"
#include "Vector4.hpp"
#include "Matrix.hpp"
//#include "Matrix2x2.hpp"
//#include "Matrix4x4.hpp"

namespace lw
{
	template<size_t N>
	using Vec = vector_t<N, f32>;

	using Vec2 = Vec<2>;
	using Vec3 = Vec<3>;
	using Vec4 = Vec<4>;
}