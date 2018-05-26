#pragma once

#include "Standard.hpp"

namespace lw
{
	class LWAPI PerlinNoise
	{
	private:
		i32 m_p[512];

	public:
		explicit PerlinNoise(u32 seed);
		void reseed(u32 seed);

		f32 noise(f32 x, f32 y, f32 z) const;
		f32 noise(f32 x, f32 y, f32 z, size_t octaves) const;
		f32 noise0_1(f32 x, f32 y, f32 z) const;
		f32 noise0_1(f32 x, f32 y, f32 z, size_t octaves) const;

	private:
		static f32 fade(f32 t) noexcept;
		static f32 lerp(f32 t, f32 a, f32 b) noexcept;
		static f32 grad(i32 hash, f32 x, f32 y, f32 z) noexcept;
	};
}