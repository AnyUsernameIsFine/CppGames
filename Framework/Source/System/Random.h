#pragma once

#include <random>

#include "System\Globals.h"

namespace Framework
{
	class Random
	{
	public:
		static void setRandSeed(UInt seed);
		static void setHashSeed(UInt32 seed);
		static int randInt();
		static int randInt(int min, int max);
		static float randFloat(float min = 0.0f, float max = 1.0f);
		static UInt32 UInt32FromByteArray(const void* byteArray, int length);

	private:
		using RNG = std::mt19937;

		static constexpr UInt32 PRIME_1 = 2654435761U;
		static constexpr UInt32 PRIME_2 = 2246822519U;
		static constexpr UInt32 PRIME_3 = 3266489917U;
		static constexpr UInt32 PRIME_4 = 668265263U;
		static constexpr UInt32 PRIME_5 = 374761393U;

		Random() {}
		static UInt32 XXH_readLE32(const void* ptr);
		static UInt32 XXH32_round(UInt32 seed, UInt32 input);

		static RNG rng;
		static UInt32 hashSeed;
	};
}
