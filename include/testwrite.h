/* simple test-value-to-stdout code
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
#pragma once
#include <stdint.h>
#include <stdio.h>

/* for 32-bit value; to be called in an infinite loop */
static inline void add_output(int32_t x) {
	static size_t outbuf_i = 0;
	static int32_t outbuf[BUFSIZ];
	outbuf[outbuf_i++] = x;
	if (outbuf_i == BUFSIZ) {
		fwrite(outbuf, sizeof(int32_t), outbuf_i, stdout);
		outbuf_i = 0;
	}
}

/* wrapper for full 64-bit value */
static inline void add_output2x(int64_t x) {
	add_output(x);
	add_output(x >> 32);
}

/* wrapper for upper half of 64-bit value */
static inline void add_output64u(int64_t x) {
	add_output(x >> 32);
}

/* wrapper for lower half of 64-bit value */
static inline void add_output64l(int64_t x) {
	add_output(x);
}
