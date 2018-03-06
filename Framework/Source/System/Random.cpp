#include "Random.h"

namespace Framework
{
	void Random::setRandSeed(UInt seed)
	{
		rng.seed(seed);
		srand(seed);
	}

	void Random::setHashSeed(UInt32 seed)
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

	UInt32 Random::UInt32FromByteArray(const void* byteArray, int length)
	{
		const UInt8* p = static_cast<const UInt8*>(byteArray);
		const UInt8* bEnd = p + length;
		UInt32 h32;

		if (length >= 16) {
			const UInt8* const limit = bEnd - 16;
			UInt32 v1 = hashSeed + PRIME_1 + PRIME_2;
			UInt32 v2 = hashSeed + PRIME_2;
			UInt32 v3 = hashSeed + 0;
			UInt32 v4 = hashSeed - PRIME_1;

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

		h32 += static_cast<UInt32>(length);

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

	UInt32 Random::XXH_readLE32(const void* ptr)
	{
		static int ONE = 1;
		static bool LITTLE_ENDIAN = *reinterpret_cast<const char*>(&ONE);

		UInt32 val;
		memcpy(&val, ptr, sizeof(val));

		return LITTLE_ENDIAN ? val : _byteswap_ulong(val);
	}

	UInt32 Random::XXH32_round(UInt32 seed, UInt32 input)
	{
		seed += input * PRIME_2;
		seed = _rotl(seed, 13);
		seed *= PRIME_1;
		return seed;
	}

	Random::RNG Random::rng;
	UInt32 Random::hashSeed;
}
