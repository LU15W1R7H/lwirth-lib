#pragma once

#include "Standard.hpp"

#include <ctime>

namespace lw
{
	template<typename FieldType, U16 N, int M, int R, int A, int F, int U, int S, int B, int T, int C, int L>
	class API MersenneTwisterBase
	{
	private:
		const U64 MASK_LOWER = (1ull << R) - 1;
		const U64 MASK_UPPER = (1ull << R);

		U16 m_index;
		FieldType m_mt[N];

		void twist()
		{
			for (U16 i = 0; i < N; i++)
			{
				U32 x = (m_mt[i] & MASK_UPPER) + (m_mt[(i + 1) % N] & MASK_LOWER);

				U32 xA = x >> 1;

				if (x & 1)
				{
					xA ^= A;
				}

				m_mt[i] = m_mt[(i + M) % N] ^ xA;
			}

			m_index = 0;
		}

	public:
		MersenneTwisterBase()
		{
			std::time_t timeStamp = std::time(nullptr);
			setSeed((FieldType)timeStamp);
		}

		MersenneTwisterBase(FieldType seed)
		{
			setSeed(seed);
		}

		void setSeed(FieldType seed)
		{
			m_mt[0] = seed;
			for (U32 i = 1; i < N; i++)
			{
				m_mt[i] = (F * (m_mt[i - 1] ^ (m_mt[i - 1] >> 30)) + i);
			}

			m_mt[28] ^= 0xBBEBBEBB;

			m_index = N;

			for (U32 i = 0; i < 9000000; i++)
			{
				next();
			}
		}

		FieldType next()
		{
			if (m_index >= N)
			{
				twist();
			}

			FieldType x = m_mt[m_index];
			m_index++;

			x ^= (x >> U);
			x ^= (x << S) & B;
			x ^= (x << T) & C;
			x ^= (x >> L);
			
			return x;
		}
	};

	API typedef MersenneTwisterBase<U32, 624, 397, 31, 0x9908B0DF, 1812433253, 11, 7, 0x9D2C5680, 15, 0xEFC60000, 18> mt19937;
	
}