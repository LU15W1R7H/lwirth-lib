#pragma once

#include "Standard.hpp"

namespace lw
{
	class API PerlinNoise
	{
	private:
		I32 m_p[512];

	public:
		explicit PerlinNoise(U32 seed);
		void reseed(U32 seed);

		F32 noise(F32 x, F32 y, F32 z) const;
		F32 noise(F32 x, F32 y, F32 z, size_t octaves) const;
		F32 noise0_1(F32 x, F32 y, F32 z) const;
		F32 noise0_1(F32 x, F32 y, F32 z, size_t octaves) const;

	private:
		static F32 fade(F32 t) noexcept;
		static F32 lerp(F32 t, F32 a, F32 b) noexcept;
		static F32 grad(I32 hash, F32 x, F32 y, F32 z) noexcept;
	};
}