/* xorshift32 test program
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
#include "testwrite.h"

/*
 * Here's xorshift32 for comparison and a quick test.
 *
 * xorshift32 also seems to work well for the seeding
 * of ranoise32 (and similar code using MUVAROR32()).
 */

/** Initial seed for xorshift32(). Other non-zero values can be used. */
#define XORSHIFT32_SEED 2463534242UL

/**
 * Get Marsaglia xorshift32 state from non-zero \p seed.
 */
static inline uint32_t xorshift32(uint32_t seed) {
	uint32_t x = seed;
	x ^= x << 13;
	x ^= x >> 17;
	x ^= x << 5; // Marsaglia's version (most common, better in dieharder)
	//x ^= x << 15; // WebDrake's version (also valid, worse in dieharder)
	return x;
}

int main(int argc, char *argv[]) {
	uint32_t s = XORSHIFT32_SEED;
	for (;;) {
		/* xorshift32 test */
		s = xorshift32(s);
		add_output(s);
	}
	return 0;
}
