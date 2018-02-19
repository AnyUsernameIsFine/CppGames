#pragma once

#ifndef MEM_MODULE
#	if !defined (__VMS) \
	&& (defined (__cplusplus) \
	|| (defined (__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) /* C99 */) )
#		include <stdint.h>
typedef		uint8_t			BYTE;
typedef		uint32_t		U32;
#	else
typedef		unsigned char	BYTE;
typedef		unsigned char	U32;
#	endif
#endif

namespace Game
{
	class Random
	{
	public:
		static void setRandSeed(U32 seed);
		static void setHashSeed(U32 seed);
		static float randFloat();
		static float randFloat(float minInclusive, float maxExclusive);
		static U32 u32FromByteArray(const void* byteArray, size_t length);

	private:
		static U32 hashSeed_;
		static const int ONE_;
		static const bool LITTLE_ENDIAN_;
		static const U32 PRIME_1_;
		static const U32 PRIME_2_;
		static const U32 PRIME_3_;
		static const U32 PRIME_4_;
		static const U32 PRIME_5_;

		Random() {}
		static U32 XXH_read32(const void* memPtr);
		static U32 XXH_readLE32(const void* ptr);
		static U32 XXH32_round(U32 seed, U32 input);
	};
}
