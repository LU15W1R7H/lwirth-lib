#include "stdafx.hpp"
#include "Random.hpp"

#include "Math.hpp"

namespace lw
{
	Random::Random()
		: m_mt()
	{
	}

	Random::Random(u32 seed)
		: m_mt(seed)
	{
	}

	Random::~Random()
	{
	}

	void Random::setSeed(u32 seed)
	{
		m_mt.setSeed(seed);
	}

	const std::string Random::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const std::string Random::DIGITS = "0123456789";
	const std::string Random::CHARS = LETTERS + DIGITS;
	


	u32 Random::nextU32()
	{
		return m_mt.next();
	}

	u32 Random::nextU32(u32 min, u32 max)
	{
		return mapRandom<u32>(min, max);
	}

	u32 Random::nextU32(u32 max)
	{
		return mapRandom<u32>(0, max);
	}

	i32 Random::nextI32(i32 min, i32 max)
	{
		return mapRandom<i32>(min, max);
	}

	i32 Random::nextI32(i32 max)
	{
		return nextI32(0, max);
	}

	f32 Random::nextF32()
	{
		return nextF32(0.f, 1.f);
	}

	f32 Random::nextF32(f32 min, f32 max)
	{
		return mapRandom<f32>(min, max);
	}

	f32 Random::nextF32(f32 max)
	{
		return nextF32(0.f, max);
	}

	f64 Random::nextD64()
	{
		return nextD64(0.0, 1.0);
	}

	f64 Random::nextD64(f64 min, f64 max)
	{
		return mapRandom<f64>(min, max);
	}

	f64 Random::nextD64(f64 max)
	{
		return nextD64(0.0, max);
	}

	bool Random::nextBool()
	{
		return nextU32() > std::numeric_limits<u32>::max() / 2;
	}

	char Random::nextChar()
	{
		return CHARS.at(nextIndex(CHARS.size()));
	}

	char Random::nextLetter()
	{
		return LETTERS.at(nextIndex(LETTERS.size()));
	}

	char Random::nextDigit()
	{
		return DIGITS.at(nextIndex(DIGITS.size()));
	}

	std::string Random::nextString(size_t length)
	{
		std::string s = "";
		for (size_t i = 0; i < length; i++)
		{
			s += nextChar();
		}
		return s;
	}

	size_t Random::nextIndex(size_t min, size_t max)
	{
		return mapRandom<size_t>(min, max - 1);
	}

	size_t Random::nextIndex(size_t max)
	{
		return nextIndex(0, max);
	}

	f32 Random::nextAngle()
	{
		return mapRandom<f32>(0.f, PI2);
	}

	Vec2 Random::nextVector2(f32 minX, f32 maxX, f32 minY, f32 maxY)
	{
		return Vec2(nextF32(minX, maxX), nextF32(minY, maxY));
	}

	Vec2 Random::nextVector2(f32 maxX, f32 maxY)
	{
		return nextVector2(0.f, maxX, 0.f, maxY);
	}

	Vec2 Random::nextVector2()
	{
		return Vec2(nextF32(), nextF32());
	}

	Vec2 Random::nextVector2(const Vec2& maxVec)
	{
		return nextVector2(maxVec.x, maxVec.y);
	}

	Color Random::nextColor()
	{
		return Color(nextF32(), nextF32(), nextF32());
	}

	Vertex2D Random::nextVertex()
	{
		return Vertex2D(nextVector2(-1.f, 1.f, -1.f, 1.f), nextColor());
	}

	Triangle Random::nextTriangle()
	{
		return lw::Triangle(
			nextVertex(),
			nextVertex(),
			nextVertex()
		);
	}


	template<typename T>
	T Random::mapRandom(T min, T max)
	{
		return map<u32, T>(nextU32(), std::numeric_limits<u32>::min(), std::numeric_limits<u32>::max(), min, max);
	}

}