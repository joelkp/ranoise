/* ranoise32 test for avalanche effect specifically
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

#define FIBH32 2654435769UL // 32-bit Fibonacci hash number

/** Rotate bits right, for 32-bit unsigned \p x, \p r positions. */
#define ROR32(x, r) \
	((uint32_t)(x) >> ((r) & 31) | ((uint32_t)(x) << (32-((r) & 31))))

/*
 * Plain ranoise32 generally does well, but it has some weaknesses to
 * the avalanche effect, such as numbers exactly a large power of two
 * producing zero output. In the simple test below, a start of 1 will
 * give a much better result than a start of 0.
 */
int32_t ranoise32(uint32_t x) {
	x *= FIBH32;
	x *= ROR32(x, x + 14);
	x ^= (x >> 7) ^ (x >> 16);
	return x;
}

/*
 * The pattern is similar, but the series of zero output values won't
 * be zero any longer. They now at least vary well in the lower bits.
 */
int32_t ranoise32b(uint32_t x) {
	x *= FIBH32;
	x = (x | 1) * ROR32(x, x + 14);
	x ^= (x >> 7) ^ (x >> 16);
	return x;
}

#define TESTFUNC ranoise32
#define TESTFROM 0

int main(void) {
	uint32_t x = TESTFROM;
	for (int i = 0; i < 32; ++i) {
		int32_t y;
		x ^= (1 << i);
		y = TESTFUNC(x);
		printf("%8x -> %8x\n", x, y);
		x ^= (1 << i);
	}
	return 0;
}
