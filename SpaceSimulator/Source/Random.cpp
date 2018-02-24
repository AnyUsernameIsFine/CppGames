#include "Random.h"

#include <stdlib.h>
#include <string.h>

namespace Game
{
	void Random::setRandSeed(unsigned int seed)
	{
		rng_.seed(seed);
		srand(seed);
	}

	void Random::setHashSeed(uint32_t seed)
	{
		hashSeed_ = seed;
	}

	int Random::randInt()
	{
		return rng_();
	}

	int Random::randInt(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rng_);
	}

	float Random::randFloat(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rng_);
	}

	Random::U32 Random::ui32FromByteArray(const void* byteArray, size_t length)
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
				v1 = XXH32_round_(v1, XXH_readLE32_(p));
				p += 4;
				v2 = XXH32_round_(v2, XXH_readLE32_(p));
				p += 4;
				v3 = XXH32_round_(v3, XXH_readLE32_(p));
				p += 4;
				v4 = XXH32_round_(v4, XXH_readLE32_(p));
				p += 4;
			} while (p <= limit);

			h32 = _rotl(v1, 1) + _rotl(v2, 7) + _rotl(v3, 12) + _rotl(v4, 18);
		}
		else {
			h32 = hashSeed_ + PRIME_5_;
		}

		h32 += (U32)length;

		while (p + 4 <= bEnd) {
			h32 += XXH_readLE32_(p) * PRIME_3_;
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
	Random::U32 Random::hashSeed_;
	const bool Random::LITTLE_ENDIAN_ = *(const char*)(&ONE_);

	Random::U32 Random::XXH_readLE32_(const void* ptr)
	{
		U32 val;
		memcpy(&val, ptr, sizeof(val));

		return LITTLE_ENDIAN_ ? val : _byteswap_ulong(val);
	}

	Random::U32 Random::XXH32_round_(U32 seed, U32 input)
	{
		seed += input * PRIME_2_;
		seed = _rotl(seed, 13);
		seed *= PRIME_1_;
		return seed;
	}
}
