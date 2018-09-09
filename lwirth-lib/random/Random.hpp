#pragma once

#include "../Standard.hpp"

#include "MersenneTwister.hpp"
#include "../general/Utils.hpp"

#include "../memory/List.hpp"
#include <string>
#include <eigen/Dense>
#include "../graphics/Color.hpp"
#include "../graphics/Vertices.hpp"
#include "../math/Triangle.hpp"

namespace lw
{
	class Random
	{
	public:
		Random();
		Random(u32 seed);
		~Random();

		void setSeed(u32 seed);

		//@return random U32 in range [0 -> U32::max]
		u32 nextU32();
		//@return random U32 in range [min -> max]
		u32 nextU32(u32 min, u32 max);
		//@return random U32 in range [0 -> max]
		u32 nextU32(u32 max);
		//@return random I32 in range [min -> max]
		i32 nextI32(i32 min, i32 max);
		//@return random I32 in range [0 -> max]
		i32 nextI32(i32 max);
		//@return random F32 in range [0.f -> 1.f]
		f32 nextF32();
		//@return random F32 in range [min -> max]
		f32 nextF32(f32 min, f32 max);
		//@return random F32 in range [0.f -> max]
		f32 nextF32(f32 max);
		//@return random F64 in range [0.0 -> 1.0]
		f64 nextD64();
		//@return random F32 in range [min -> max]
		f64 nextD64(f64 min, f64 max);
		//@return random F32 in range [0.0 -> max]
		f64 nextD64(f64 max);
		//@return random bool
		bool nextBool();
		//@return random char(letters & digits)
		char nextChar();
		//@return random char(letter)
		char nextLetter();
		//@return random char(digit)
		char nextDigit();
		//@return random std::string containing letters & digits
		std::string nextString(size_t length);
		//@return random size_t(index) in range [min -> max(exlusive)]
		size_t nextIndex(size_t min, size_t max);
		//@return random size_t(index) in range [0 -> max(exlusive)]
		size_t nextIndex(size_t max);

		template<typename T>
		const T& nextElement(const lw::List<T>& elements)
		{
			return elements.at(nextIndex(elements.size()));
		}

		template<typename T>
		T& nextElement(const T* elements, size_t size)
		{
			return elements[nextIndex(size)];
		}

		//@return random F32(angle) in radians in range [0 -> 2PI]
		f32 nextAngle();

		//@return random lw::Color
		Color nextColor();

		Eigen::MatrixXf nextMatrix(size_t rows, size_t cols, f32 min, f32 max);

		Eigen::VectorXf nextVector(size_t dim, f32 min, f32 max);

		Vertex2D nextVertex2D();


	private:
		mt19937 m_mt;
		
		template<typename T>
		T mapRandom(T min, T max);

		static const std::string LETTERS;
		static const std::string DIGITS;
		static const std::string CHARS;
	};
}