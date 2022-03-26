/* ranoise32b test program
 * Copyright (c) 2022 Joel K. Pettersson
 * <joelkpettersson@gmail.com>.
 *
 * Permission to use, copy, modify, and/or distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */
#include <stdio.h>
#include <stdint.h>

#define SAU_FIBH32                2654435769UL // 32-bit Fibonacci hash number

/** Rotate bits left, for 32-bit unsigned \p x, \p r positions. */
#define SAU_ROL32(x, r) \
	((uint32_t)(x) << ((r) & 31) | ((uint32_t)(x) >> (32-((r) & 31))))

/** Rotate bits right, for 32-bit unsigned \p x, \p r positions. */
#define SAU_ROR32(x, r) \
	((uint32_t)(x) >> ((r) & 31) | ((uint32_t)(x) << (32-((r) & 31))))

/**
 * Random access noise. Chaotic waveshaper which turns evenly spaced, and other
 * simple, number sequences into something similar to white noise. Returns zero
 * for zero. The frequency spectrum when used with a counter is flat. This is a
 * slightly more complicated version of SAU_ranoise32(), and passes more tests.
 * It's more generally suitable as a non-cryptographic random number generator.
 *
 * This function is mainly an alternative to using buffers of noise, for random
 * access. The index \p n can be used as a counter or varied for random access.
 *
 * \return pseudo-random number for index \p n
 */
static inline int32_t SAU_ranoise32b(uint32_t n) {
	uint32_t s = n * SAU_FIBH32;
	/*
	 * 14 as offset number and other constants are currently simply copied.
	 * Further testing remains to be done to check whether they are optimal
	 * or best changed, for ranoise32b() compared to the older ranoise32().
	 */
	s = (s | 1) * SAU_ROR32(s, s + 14);
	s ^= (s >> 7) ^ (s >> 16); // improve worse lower bits with higher bits
	return s;
}

int main(int argc, char *argv[]) {
	int i = 0;
	for (;;) {
		/* chaos whaveshaper test */
		uint32_t x = i++;
		putw(SAU_ranoise32b(x), stdout);
	}
	return 0;
}
