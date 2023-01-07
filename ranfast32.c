/* ranfast32 test program
 * Copyright (c) 2023 Joel K. Pettersson
 * <joelkpettersson@gmail.com>.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include "muvaror32.h"
#include "testwrite.h"

/**
 * Random access noise, fast version with bitshifts but without rotation.
 *
 * Related to the higher-quality versions of ranoise32; it consititutes a
 * kind of weakened version, which was tested with TestU01 SmallCrush for
 * finding the best bitshift constants for the "real" ranoise32 versions.
 *
 * This function is mainly an alternative to using buffers of noise, for random
 * access. The index \p n can be used as a counter or varied for random access.
 *
 * \return pseudo-random number for index \p n
 */
static inline int32_t ranfast32(uint32_t n) {
	uint32_t s = n * FIBH32;
	s ^= s >> 14;
	s = (s | 1) * s;
	s ^= s >> 13;
	return s;
}

/**
 * Random access noise, fast version with bitshifts but without rotation.
 *
 * Related to the higher-quality versions of ranoise32; it consititutes a
 * kind of weakened version, which was tested with TestU01 SmallCrush for
 * finding the best bitshift constants for the "real" ranoise32 versions.
 *
 * This "next" function returns a new value each time, corresponding to a state
 * \p pos, which is increased. It may be initialized with any seed (0 is fine).
 *
 * \return pseudo-random number for state \p pos
 */
static inline int32_t ranfast32_next(uint32_t *restrict pos) {
	uint32_t s = *pos += FIBH32;
	s ^= s >> 14;
	s = (s | 1) * s;
	s ^= s >> 13;
	return s;
}

int main(int argc, char *argv[]) {
	int i = 0;
	for (;;) {
		/* chaos waveshaper test */
		uint32_t x = i++;
		add_output(ranfast32(x));
	}
	return 0;
}
