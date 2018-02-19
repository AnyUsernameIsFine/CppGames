#include "Random.h"

#include <stdlib.h>
#include <string.h>

namespace Game
{
	void Random::setRandSeed(U32 seed)
	{
		rng_.seed(seed);
	}

	void Random::setHashSeed(U32 seed)
	{
		hashSeed_ = seed;
	}

	float Random::randFloat(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rng_);
	}

	int Random::randInt(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rng_);
	}

	U32 Random::u32FromByteArray(const void* byteArray, size_t length)
	{
		const BYTE* p = (const BYTE*)byteArray;
		const BYTE* bEnd = p + length;
		U32 h32;

		if (length >= 16) {
			const BYTE* const limit = bEnd - 16;
			U32 v1 = hashSeed_ + PRIME_1_ + PRIME_2_;
			U32 v2 = hashSeed_ + PRIME_2_;
			U32 v3 = hashSeed_ + 0;
			U32 v4 = hashSeed_ - PRIME_1_;

			do {
				v1 = XXH32_round(v1, XXH_readLE32(p));
				p += 4;
				v2 = XXH32_round(v2, XXH_readLE32(p));
				p += 4;
				v3 = XXH32_round(v3, XXH_readLE32(p));
				p += 4;
				v4 = XXH32_round(v4, XXH_readLE32(p));
				p += 4;
			} while (p <= limit);

			h32 = _rotl(v1, 1) + _rotl(v2, 7) + _rotl(v3, 12) + _rotl(v4, 18);
		}
		else {
			h32 = hashSeed_ + PRIME_5_;
		}

		h32 += (U32)length;

		while (p + 4 <= bEnd) {
			h32 += XXH_readLE32(p) * PRIME_3_;
			h32 = _rotl(h32, 17) * PRIME_4_;
			p += 4;
		}

		while (p < bEnd) {
			h32 += (*p) * PRIME_5_;
			h32 = _rotl(h32, 11) * PRIME_1_;
			p++;
		}

		h32 ^= h32 >> 15;
		h32 *= PRIME_2_;
		h32 ^= h32 >> 13;
		h32 *= PRIME_3_;
		h32 ^= h32 >> 16;

		return h32;
	}

	Random::RNG Random::rng_;
	U32 Random::hashSeed_;
	const int Random::ONE_ = 1;
	const bool Random::LITTLE_ENDIAN_ = *(const char*)(&ONE_);
	const U32 Random::PRIME_1_ = 2654435761U;
	const U32 Random::PRIME_2_ = 2246822519U;
	const U32 Random::PRIME_3_ = 3266489917U;
	const U32 Random::PRIME_4_ = 668265263U;
	const U32 Random::PRIME_5_ = 374761393U;

	U32 Random::XXH_read32(const void* memPtr)
	{
		U32 val;
		memcpy(&val, memPtr, sizeof(val));
		return val;
	}

	U32 Random::XXH_readLE32(const void* ptr)
	{
		return LITTLE_ENDIAN_ ? XXH_read32(ptr) : _byteswap_ulong(XXH_read32(ptr));
	}

	U32 Random::XXH32_round(U32 seed, U32 input)
	{
		seed += input * PRIME_2_;
		seed = _rotl(seed, 13);
		seed *= PRIME_1_;
		return seed;
	}
}
