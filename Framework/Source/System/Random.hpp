#pragma once

#include "System\Globals.hpp"

#include <random>

namespace Framework
{
	class Random
	{
		typedef std::mt19937 RNG;

	public:
		static void setRandSeed(uInt seed);
		static void setHashSeed(uInt32 seed);
		static int randInt();
		static int randInt(int min, int max);
		static float randFloat(float min = 0, float max = 1);
		static uInt32 uInt32FromByteArray(const void* byteArray, uInt length);

	private:
		static RNG rng;
		static uInt32 hashSeed;
		static const int ONE = 1;
		static const bool LITTLE_ENDIAN;
		static const uInt32 PRIME_1 = 2654435761U;
		static const uInt32 PRIME_2 = 2246822519U;
		static const uInt32 PRIME_3 = 3266489917U;
		static const uInt32 PRIME_4 = 668265263U;
		static const uInt32 PRIME_5 = 374761393U;

		Random() {}
		static uInt32 XXH_readLE32(const void* ptr);
		static uInt32 XXH32_round(uInt32 seed, uInt32 input);
	};
}
