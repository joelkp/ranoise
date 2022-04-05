/* 64-bit LCG test program
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
 * Here's a 64-bit LCG for comparison and quick test.
 *
 * Upper half of value passes TestU01 SmallCrush, but
 * lower half fails like 32-bit LCG; full also fails.
 */
#define add_output64(x) add_output2x(x)
//#define add_output64(x) add_output64u(x)
//#define add_output64(x) add_output64l(x)

/** 64-bit multiplier for 64-bit LCG picked as best by Steele & Vigna, 2021. */
#define LCG_A64 0xd1342543de82ef95

/**
 * Get next 64-bit LCG state from prior \p x.
 */
static inline uint64_t lcg64_next(uint64_t x) {
	return (x * LCG_A64) + 1;
}

int main(int argc, char *argv[]) {
	uint64_t s = 0;
	for (;;) {
		/* 64-bit LCG test */
		s = lcg64_next(s);
		add_output64(s);
	}
	return 0;
}
