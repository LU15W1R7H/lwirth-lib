#pragma once

#include "Standard.hpp"

#include "MersenneTwister.hpp"
#include "Utils.hpp"

#include "DynamicArray.hpp"
#include <string>
#include "Vector2.hpp"
#include "Color.hpp"
#include "Vertices.hpp"
#include "Triangle.hpp"

namespace lw
{
	class API Random
	{
	public:
		Random();
		Random(U32 seed);
		~Random();

		void setSeed(U32 seed);

		//@return random U32 in range [0 -> U32::max]
		U32 nextU32();
		//@return random U32 in range [min -> max]
		U32 nextU32(U32 min, U32 max);
		//@return random U32 in range [0 -> max]
		U32 nextU32(U32 max);
		//@return random I32 in range [min -> max]
		I32 nextI32(I32 min, I32 max);
		//@return random I32 in range [0 -> max]
		I32 nextI32(I32 max);
		//@return random F32 in range [0.f -> 1.f]
		F32 nextF32();
		//@return random F32 in range [min -> max]
		F32 nextF32(F32 min, F32 max);
		//@return random F32 in range [0.f -> max]
		F32 nextF32(F32 max);
		//@return random F64 in range [0.0 -> 1.0]
		D64 nextD64();
		//@return random F32 in range [min -> max]
		D64 nextD64(D64 min, D64 max);
		//@return random F32 in range [0.0 -> max]
		D64 nextD64(D64 max);
		//@return random bool
		bool nextBool();
		//@return random char(letters & digits)
		char nextChar();
		//@return random char(letter)
		char nextLetter();
		//@return random char(digit)
		char nextDigit();
		//@return random std::string containing letters & digits
		std::string nextString(Size length);
		//@return random Size(index) in range [min -> max(exlusive)]
		Size nextIndex(Size min, Size max);
		//@return random Size(index) in range [0 -> max(exlusive)]
		Size nextIndex(Size max);

		template<typename T>
		const T& nextElement(const lw::DynamicArray<T>& elements)
		{
			return elements.at(nextIndex(elements.size()));
		}

		template<typename T>
		T& nextElement(const T* elements, Size size)
		{
			return elements[nextIndex(size)];
		}

		//@return random F32(angle) in radians in range [0 -> 2PI]
		Angle nextAngle();

		//@return random lw::Vector2
		Vector2 nextVector2(F32 minX, F32 maxX, F32 minY, F32 maxY);
		//@return random lw::Vector2
		Vector2 nextVector2(F32 maxX, F32 maxY);
		Vector2 nextVector2();
		//@return random lw::Vector2
		Vector2 nextVector2(const Vector2& maxVec);
		//@return random lw::Color
		Color nextColor();
		Vertex nextVertex();
		Triangle nextTriangle();

	private:
		mt19937 m_mt;
		
		template<typename T>
		T mapRandom(T min, T max);

		static const std::string LETTERS;
		static const std::string DIGITS;
		static const std::string CHARS;
	};
}