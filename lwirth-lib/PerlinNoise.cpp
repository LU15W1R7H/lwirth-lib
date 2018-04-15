#include "stdafx.hpp"
#include "PerlinNoise.hpp"

#include <algorithm>
#include <random>

namespace lw
{
	PerlinNoise::PerlinNoise(U32 seed)
	{
		reseed(seed);
	}

	void PerlinNoise::reseed(U32 seed)
	{
		for (size_t i = 0; i < 256; i++)
		{
			m_p[i] = i;
		}

		std::shuffle(std::begin(m_p), std::begin(m_p) + 256, std::default_random_engine(seed));

		for (size_t i = 0; i < 256; i++)
		{
			m_p[256 + i] = m_p[i];
		}
	}

	F32 PerlinNoise::noise(F32 x, F32 y, F32 z) const
	{
		const I32 X = static_cast<I32>(std::floor(x)) & 255;
		const I32 Y = static_cast<I32>(std::floor(y)) & 255;
		const I32 Z = static_cast<I32>(std::floor(z)) & 255;

		x -= std::floor(x);
		y -= std::floor(y);
		z -= std::floor(z);

		const F32 u = fade(x);
		const F32 v = fade(y);
		const F32 w = fade(z);

		const I32 A = m_p[X] + Y, AA = m_p[A] + Z, AB = m_p[A + 1] + Z;
		const U32 B = m_p[X + 1] + Y, BA = m_p[B] + Z, BB = m_p[B + 1] + Z;

		return lerp(w, lerp(v, lerp(u, grad(m_p[AA], x, y, z),
			grad(m_p[BA], x - 1.f, y, z)),
			lerp(u, grad(m_p[AB], x, y - 1.f, z),
				grad(m_p[BB], x - 1.f, y - 1.f, z))),
			lerp(v, lerp(u, grad(m_p[AA + 1], x, y, z - 1.f),
				grad(m_p[BA + 1], x - 1.f, y, z - 1.f)),
				lerp(u, grad(m_p[AB + 1], x, y - 1.f, z - 1.f),
					grad(m_p[BB + 1], x - 1.f, y - 1.f, z - 1.f))));
	}

	F32 PerlinNoise::noise(F32 x, F32 y, F32 z, size_t octaves) const
	{
		float result = 0.0f;
		float amp = 1.0f;

		for (size_t i = 0; i < octaves; ++i)
		{
			result += noise(x, y, z) * amp;
			x *= 2.0f;
			y *= 2.0f;
			z *= 2.0f;
			amp *= 0.5f;
		}

		return result;
	}

	F32 PerlinNoise::noise0_1(F32 x, F32 y, F32 z) const
	{
		return noise(x, y, z) * 0.5f + 0.5f;
	}

	F32 PerlinNoise::noise0_1(F32 x, F32 y, F32 z, size_t octaves) const
	{
		return noise(x, y, z, octaves) * 0.5f + 0.5f;
	}

	F32 PerlinNoise::fade(F32 t) noexcept
	{
		return t * t * t * (t * (t * 6 - 15) + 10);
	}

	F32 PerlinNoise::lerp(F32 t, F32 a, F32 b) noexcept
	{
		return a + t * (b - a);
	}

	F32 PerlinNoise::grad(I32 hash, F32 x, F32 y, F32 z) noexcept
	{
		const I32 h = hash & 15;
		const F32 u = h < 8 ? x : y;
		const F32 v = h < 4 ? x : (h == 12 || h == 14 ? x : z);
		return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
	}

}


