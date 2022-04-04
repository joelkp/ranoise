/* 32-bit LCG test program
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
 * Here's a 32-bit LCG for comparison and quick test.
 */

/** 32-bit multiplier for 32-bit LCG picked as best by Steele & Vigna, 2021. */
#define LCG_A32 0x915f77f5

/**
 * Get next 32-bit LCG state from prior \p x.
 */
static inline uint32_t lcg32_next(uint32_t x) {
	return (x * LCG_A32) + 1;
}

int main(int argc, char *argv[]) {
	uint32_t s = 0;
	for (;;) {
		/* 32-bit LCG test */
		s = lcg32_next(s);
		add_output(s);
	}
	return 0;
}
