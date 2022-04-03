/* ranoise32 test for avalanche effect specifically
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
#include <stdio.h>
#include "muvaror32.h"

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
 * By comparison ranoise32b does better with avalanching. For numbers
 * that are large powers of two, gaps with too-low values are filled,
 * by setting the bit which corresponds to the bitrotation offset for
 * the multiplier. Additionally, when the 1 bit is set avalanching of
 * even numbers improves, especially for large numbers.
 */
int32_t ranoise32b(uint32_t x) {
	x *= FIBH32;
	x ^= x >> 14;
	x = MUVAROR32(x, x >> 27, 0);
	x ^= x >> 13;
	return x;
}

#define TESTFUNC ranoise32b
#define TESTFROM 0 //12345

int main(void) {
	uint32_t x = TESTFROM;
	for (int i = 0; i < 32; ++i) {
		int32_t y;
		x ^= (1 << i);
		y = TESTFUNC(x);
		printf("%08x -> %08x\n", x, y);
//		printf("%032b -> %032b\n", x, y);
		x ^= (1 << i);
	}
	return 0;
}
