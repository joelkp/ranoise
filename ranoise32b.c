/* ranoise32b test program
 * Copyright (c) 2022 Joel K. Pettersson
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
 * Random access noise. Chaotic waveshaper which turns evenly spaced, and other
 * simple, number sequences into white noise. Returns zero for zero. This is an
 * improved version of ranoise32_old(), which passes more statistical tests and
 * with small overhead is more suitable for general non-cryptographic purposes.
 *
 * This function is mainly an alternative to using buffers of noise, for random
 * access. The index \p n can be used as a counter or varied for random access.
 *
 * \return pseudo-random number for index \p n
 */
static inline int32_t ranoise32b(uint32_t n) {
	uint32_t s = n * FIBH32;
	s ^= s >> 14;
	s = MUVAROR32(s, s >> 27, 0);
	s ^= s >> 13;
	return s;
}

/**
 * Random access noise. Chaotic waveshaper which turns evenly spaced, and other
 * simple, number sequences into white noise. Returns zero for zero. This is an
 * improved version of ranoise32_old(), which passes more statistical tests and
 * with small overhead is more suitable for general non-cryptographic purposes.
 *
 * This "next" function returns a new value each time, corresponding to a state
 * \p pos, which is increased. It may be initialized with any seed (0 is fine).
 *
 * \return pseudo-random number for state \p pos
 */
static inline int32_t ranoise32b_next(uint32_t *restrict pos) {
	uint32_t s = *pos += FIBH32;
	s ^= s >> 14;
	s = MUVAROR32(s, s >> 27, 0);
	s ^= s >> 13;
	return s;
}

int main(int argc, char *argv[]) {
	int i = 0;
	for (;;) {
		/* chaos waveshaper test */
		uint32_t x = i++;
		add_output(ranoise32b(x));
	}
	return 0;
}
