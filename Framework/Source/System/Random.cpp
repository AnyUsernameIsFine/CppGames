#include "Random.hpp"

//#include <stdlib.h>
//#include <string.h>

namespace Framework
{
	void Random::setRandSeed(uint seed)
	{
		rng.seed(seed);
		srand(seed);
	}

	void Random::setHashSeed(uint32 seed)
	{
		hashSeed = seed;
	}

	int Random::randInt()
	{
		return rng();
	}

	int Random::randInt(int min, int max)
	{
		std::uniform_int_distribution<int> distribution(min, max);
		return distribution(rng);
	}

	float Random::randFloat(float min, float max)
	{
		std::uniform_real_distribution<float> distribution(min, max);
		return distribution(rng);
	}

	uint32 Random::uint32FromByteArray(const void* byteArray, uint length)
	{
		const uint8* p = (const uint8*)byteArray;
		const uint8* bEnd = p + length;
		uint32 h32;

		if (length >= 16) {
			const uint8* const limit = bEnd - 16;
			uint32 v1 = hashSeed + PRIME_1 + PRIME_2;
			uint32 v2 = hashSeed + PRIME_2;
			uint32 v3 = hashSeed + 0;
			uint32 v4 = hashSeed - PRIME_1;

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
			h32 = hashSeed + PRIME_5;
		}

		h32 += (uint32)length;

		while (p + 4 <= bEnd) {
			h32 += XXH_readLE32(p) * PRIME_3;
			h32 = _rotl(h32, 17) * PRIME_4;
			p += 4;
		}

		while (p < bEnd) {
			h32 += (*p) * PRIME_5;
			h32 = _rotl(h32, 11) * PRIME_1;
			p++;
		}

		h32 ^= h32 >> 15;
		h32 *= PRIME_2;
		h32 ^= h32 >> 13;
		h32 *= PRIME_3;
		h32 ^= h32 >> 16;

		return h32;
	}

	Random::RNG Random::rng;
	uint32 Random::hashSeed;
	const bool Random::LITTLE_ENDIAN = *(const char*)(&ONE);

	uint32 Random::XXH_readLE32(const void* ptr)
	{
		uint32 val;
		memcpy(&val, ptr, sizeof(val));

		return LITTLE_ENDIAN ? val : _byteswap_ulong(val);
	}

	uint32 Random::XXH32_round(uint32 seed, uint32 input)
	{
		seed += input * PRIME_2;
		seed = _rotl(seed, 13);
		seed *= PRIME_1;
		return seed;
	}
}
