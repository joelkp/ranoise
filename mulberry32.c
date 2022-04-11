/* Mulberry32 test program
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
#include <stdint.h>

/*
 * Here's Mulberry32 for comparison and a quick test.
 *
 * Adapted from Tommy Ettinger's 2nd 2017 version, at
 * <https://gist.github.com/tommyettinger/46a874533244883189143505d203312c>.
 */

/**
 * Get next value for and update \p pos.
 */
static inline uint32_t mulberry32_next(uint32_t *restrict pos) {
	uint32_t z = *pos += 0x6D2B79F5UL;
	z = (z ^ (z >> 15)) * (z | 1UL);
	z ^= z + (z ^ (z >> 7)) * (z | 61UL);
	return z ^ (z >> 14);
}

int main(int argc, char *argv[]) {
	uint32_t pos = 0;
	for (;;) {
		/* SplitMix32 test */
		add_output(mulberry32_next(&pos));
	}
	return 0;
}
