/* SplitMix32b (alternative version) test program
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
 * Here's SplitMix32b, a modification which uses more
 * optimal integer hashing, for comparison and tests.
 */

/*
 * 32-bit integer-to-integer hash function with lower
 * bias than the MurmurHash3 32-bit function normally
 * used in SplitMix32.
 *
 * Function from Christopher Wellons' hash-prospector project,
 * <https://github.com/skeeto/hash-prospector>. The selection:
 *
 * [15 d168aaad 15 af723597 15] = 0.15983776156606694
 */
uint32_t
lowbias32(uint32_t x)
{
    x ^= x >> 15;
    x *= 0xd168aaad;
    x ^= x >> 15;
    x *= 0xaf723597;
    x ^= x >> 15;
    return x;
}

/*
 * In my TestU01 tests, the usual increment based on the golden ratio
 * did not work well here (SmallCrush failures), so for this function
 * the constant is (simple arithmetic) gold minus silver plus bronze.
 *
 * (A constant simply based on the silver ratio was also tried but in
 * BigCrush results were poorer than expected, hence this was tried.)
 */
#define ALTH32 2175813527UL /* 2654435769UL - 1779033703UL + 1300411461UL */

/**
 * Get next value for and update \p pos.
 */
static inline uint32_t splitmix32b(uint32_t *restrict pos) {
	uint32_t s = *pos += ALTH32;
	return lowbias32(s);
}

int main(int argc, char *argv[]) {
	uint32_t pos = 0;
	for (;;) {
		/* SplitMix32b test */
		add_output(splitmix32b(&pos));
	}
	return 0;
}
