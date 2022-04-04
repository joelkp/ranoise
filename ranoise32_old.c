/* ranoise32 -- old version -- test program
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
 * simple, number sequences into something similar to white noise. Returns zero
 * for zero. The frequency spectrum when used with a counter is flat. The third
 * part is necessary in order to make the resulting pseudo-randomness passable,
 * for more general non-cryptographic purposes. (Without it there is still e.g.
 * fairness if used for dice throws and properties of sums of numbers are fine,
 * but lower bits become so much worse than higher ones that the bits and bytes
 * extracted from the output, if used as sequences of such, give poor results.)
 *
 * This function is mainly an alternative to using buffers of noise, for random
 * access. The index \p n can be used as a counter or varied for random access.
 *
 * \return pseudo-random number for index \p n
 */
static inline int32_t ranoise32_old(uint32_t n) {
	uint32_t s = n * FIBH32;
	/*
	 * 14 below appears a good offset number. For a high-quality result, it
	 * may be best to use a number around 16 in 8-25 inclusive. Statistical
	 * testing shows 5-27 as the maximal range beyond which Diehard Squeeze
	 * fails. Subtle audio qualities vary with the number; 14 seems smooth.
	 */
	s *= ROR32(s, s + 14);
	s ^= (s >> 7) ^ (s >> 16); // improve worse lower bits with higher bits
	return s;
}

/**
 * Random access noise. Chaotic waveshaper which turns evenly spaced, and other
 * simple, number sequences into something similar to white noise. Returns zero
 * for zero. The frequency spectrum when used with a counter is flat. The third
 * part is necessary in order to make the resulting pseudo-randomness passable,
 * for more general non-cryptographic purposes. (Without it there is still e.g.
 * fairness if used for dice throws and properties of sums of numbers are fine,
 * but lower bits become so much worse than higher ones that the bits and bytes
 * extracted from the output, if used as sequences of such, give poor results.)
 *
 * This "next" function returns a new value each time, corresponding to a state
 * \p pos, which is increased. It may be initialized with any seed (0 is fine).
 *
 * \return pseudo-random number for state \p pos
 */
static inline int32_t ranoise32_old_next(uint32_t *restrict pos) {
	uint32_t s = *pos += FIBH32;
	/*
	 * 14 below appears a good offset number. For a high-quality result, it
	 * may be best to use a number around 16 in 8-25 inclusive. Statistical
	 * testing shows 5-27 as the maximal range beyond which Diehard Squeeze
	 * fails. Subtle audio qualities vary with the number; 14 seems smooth.
	 */
	s *= ROR32(s, s + 14);
	s ^= (s >> 7) ^ (s >> 16); // improve worse lower bits with higher bits
	return s;
}

int main(int argc, char *argv[]) {
	int i = 0;
	for (;;) {
		/* chaos whaveshaper test */
		uint32_t x = i++;
		add_output(ranoise32_old(x));
	}
	return 0;
}
