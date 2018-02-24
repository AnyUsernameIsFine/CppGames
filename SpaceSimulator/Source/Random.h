#pragma once

#include <random>

namespace Game
{
	class Random
	{
		typedef std::mt19937 RNG;
		typedef	uint8_t BYTE;
		typedef	uint32_t U32;

	public:
		static void setRandSeed(unsigned int seed);
		static void setHashSeed(uint32_t seed);
		static int randInt();
		static int randInt(int min, int max);
		static float randFloat(float min = 0, float max = 1);
		static U32 ui32FromByteArray(const void* byteArray, size_t length);

	private:
		static RNG rng_;
		static U32 hashSeed_;
		static const int ONE_ = 1;
		static const bool LITTLE_ENDIAN_;
		static const U32 PRIME_1_ = 2654435761U;
		static const U32 PRIME_2_ = 2246822519U;
		static const U32 PRIME_3_ = 3266489917U;
		static const U32 PRIME_4_ = 668265263U;
		static const U32 PRIME_5_ = 374761393U;

		Random() {}
		static U32 XXH_readLE32_(const void* ptr);
		static U32 XXH32_round_(U32 seed, U32 input);
	};
}
