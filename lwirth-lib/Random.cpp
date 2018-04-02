#include "stdafx.hpp"
#include "Random.hpp"

#include "Math.hpp"

namespace lw
{
	Random::Random()
		: m_mt()
	{
	}

	Random::Random(U32 seed)
		: m_mt(seed)
	{
	}

	Random::~Random()
	{
	}

	void Random::setSeed(U32 seed)
	{
		m_mt.setSeed(seed);
	}

	const std::string Random::LETTERS = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const std::string Random::DIGITS = "0123456789";
	const std::string Random::CHARS = LETTERS + DIGITS;
	


	U32 Random::nextU32()
	{
		return m_mt.next();
	}

	U32 Random::nextU32(U32 min, U32 max)
	{
		return mapRandom<U32>(min, max);
	}

	U32 Random::nextU32(U32 max)
	{
		return mapRandom<U32>(0, max);
	}

	I32 Random::nextI32(I32 min, I32 max)
	{
		return mapRandom<I32>(min, max);
	}

	I32 Random::nextI32(I32 max)
	{
		return nextI32(0, max);
	}

	F32 Random::nextF32()
	{
		return nextF32(0.f, 1.f);
	}

	F32 Random::nextF32(F32 min, F32 max)
	{
		return mapRandom<F32>(min, max);
	}

	F32 Random::nextF32(F32 max)
	{
		return nextF32(0.f, max);
	}

	D64 Random::nextD64()
	{
		return nextD64(0.0, 1.0);
	}

	D64 Random::nextD64(D64 min, D64 max)
	{
		return mapRandom<D64>(min, max);
	}

	D64 Random::nextD64(D64 max)
	{
		return nextD64(0.0, max);
	}

	bool Random::nextBool()
	{
		return nextU32() > std::numeric_limits<U32>::max() / 2;
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

	std::string Random::nextString(Size length)
	{
		std::string s = "";
		for (Size i = 0; i < length; i++)
		{
			s += nextChar();
		}
		return s;
	}

	Size Random::nextIndex(Size min, Size max)
	{
		return mapRandom<Size>(min, max - 1);
	}

	Size Random::nextIndex(Size max)
	{
		return nextIndex(0, max);
	}

	F32 Random::nextAngle()
	{
		return mapRandom<F32>(0.f, PI2);
	}

	Vec2 Random::nextVector2(F32 minX, F32 maxX, F32 minY, F32 maxY)
	{
		return Vec2(nextF32(minX, maxX), nextF32(minY, maxY));
	}

	Vec2 Random::nextVector2(F32 maxX, F32 maxY)
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
		return map<U32, T>(nextU32(), std::numeric_limits<U32>::min(), std::numeric_limits<U32>::max(), min, max);
	}

}