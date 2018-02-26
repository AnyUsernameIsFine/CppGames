#pragma once

#include "Globals.hpp"

#include <random>

namespace Framework
{
	class Random
	{
		typedef std::mt19937 RNG;

	public:
		static void setRandSeed(uint seed);
		static void setHashSeed(uint32 seed);
		static int randInt();
		static int randInt(int min, int max);
		static float randFloat(float min = 0, float max = 1);
		static uint32 uint32FromByteArray(const void* byteArray, uint length);

	private:
		static RNG rng;
		static uint32 hashSeed;
		static const int ONE = 1;
		static const bool LITTLE_ENDIAN;
		static const uint32 PRIME_1 = 2654435761U;
		static const uint32 PRIME_2 = 2246822519U;
		static const uint32 PRIME_3 = 3266489917U;
		static const uint32 PRIME_4 = 668265263U;
		static const uint32 PRIME_5 = 374761393U;

		Random() {}
		static uint32 XXH_readLE32(const void* ptr);
		static uint32 XXH32_round(uint32 seed, uint32 input);
	};
}
