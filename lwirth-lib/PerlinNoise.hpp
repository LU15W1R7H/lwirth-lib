# pragma once
# include <cstdint>
# include <numeric>
# include <algorithm>
# include <random>


namespace siv
{
	class PerlinNoise
	{
	private:

		std::int32_t p[512];

		static float Fade(float t) noexcept
		{
			return t * t * t * (t * (t * 6 - 15) + 10);
		}

		static float Lerp(float t, float a, float b) noexcept
		{
			return a + t * (b - a);
		}

		static float Grad(std::int32_t hash, float x, float y, float z) noexcept
		{
			const std::int32_t h = hash & 15;
			const float u = h < 8 ? x : y;
			const float v = h < 4 ? y : h == 12 || h == 14 ? x : z;
			return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v);
		}

	public:

		explicit PerlinNoise(std::uint32_t seed = std::default_random_engine::default_seed)
		{
			reseed(seed);
		}

		void reseed(std::uint32_t seed)
		{
			for (Size i = 0; i < 256; ++i)
			{
				p[i] = i;
			}

			std::shuffle(std::begin(p), std::begin(p) + 256, std::default_random_engine(seed));

			for (Size i = 0; i < 256; ++i)
			{
				p[256 + i] = p[i];
			}
		}

		float noise(float x) const
		{
			return noise(x, 0.0, 0.0);
		}

		float noise(float x, float y) const
		{
			return noise(x, y, 0.0);
		}

		float noise(float x, float y, float z) const
		{
			const std::int32_t X = static_cast<std::int32_t>(std::floor(x)) & 255;
			const std::int32_t Y = static_cast<std::int32_t>(std::floor(y)) & 255;
			const std::int32_t Z = static_cast<std::int32_t>(std::floor(z)) & 255;

			x -= std::floor(x);
			y -= std::floor(y);
			z -= std::floor(z);

			const float u = Fade(x);
			const float v = Fade(y);
			const float w = Fade(z);

			const std::int32_t A = p[X] + Y, AA = p[A] + Z, AB = p[A + 1] + Z;
			const std::int32_t B = p[X + 1] + Y, BA = p[B] + Z, BB = p[B + 1] + Z;

			return Lerp(w, Lerp(v, Lerp(u, Grad(p[AA], x, y, z),
				Grad(p[BA], x - 1.f, y, z)),
				Lerp(u, Grad(p[AB], x, y - 1.f, z),
				Grad(p[BB], x - 1.f, y - 1.f, z))),
				Lerp(v, Lerp(u, Grad(p[AA + 1], x, y, z - 1.f),
				Grad(p[BA + 1], x - 1.f, y, z - 1.f)),
				Lerp(u, Grad(p[AB + 1], x, y - 1.f, z - 1.f),
				Grad(p[BB + 1], x - 1.f, y - 1.f, z - 1.f))));
		}

		float octaveNoise(float x, std::int32_t octaves) const
		{
			float result = 0.0f;
			float amp = 1.0f;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x) * amp;
				x *= 2.0f;
				amp *= 0.5f;
			}

			return result;
		}

		float octaveNoise(float x, float y, std::int32_t octaves) const
		{
			float result = 0.0f;
			float amp = 1.0f;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x, y) * amp;
				x *= 2.0f;
				y *= 2.0f;
				amp *= 0.5f;
			}

			return result;
		}

		float octaveNoise(float x, float y, float z, std::int32_t octaves) const
		{
			float result = 0.0f;
			float amp = 1.0f;

			for (std::int32_t i = 0; i < octaves; ++i)
			{
				result += noise(x, y, z) * amp;
				x *= 2.0f;
				y *= 2.0f;
				z *= 2.0f;
				amp *= 0.5f;
			}

			return result;
		}

		float noise0_1(float x) const
		{
			return noise(x) * 0.5f + 0.5f;
		}

		float noise0_1(float x, float y) const
		{
			return noise(x, y) * 0.5f + 0.5f;
		}

		float noise0_1(float x, float y, float z) const
		{
			return noise(x, y, z) * 0.5f + 0.5f;
		}

		float octaveNoise0_1(float x, std::int32_t octaves) const
		{
			return octaveNoise(x, octaves) * 0.5f + 0.5f;
		}

		float octaveNoise0_1(float x, float y, std::int32_t octaves) const
		{
			return octaveNoise(x, y, octaves) * 0.5f + 0.5f;
		}

		float octaveNoise0_1(float x, float y, float z, std::int32_t octaves) const
		{
			return octaveNoise(x, y, z, octaves) * 0.5f + 0.5f;
		}
	};
}
