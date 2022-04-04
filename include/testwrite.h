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
#include <stdio.h>

/* to be called in an infinite loop */
static inline void add_output(int32_t x) {
#if 1
	/*
	 * This can be the fastest option on Linux with glibc,
	 * because there's already a buffer used under the hood.
	 */
	putw(x, stdout);
#else
	/*
	 * This is the portable option, to be used if putw() is
	 * missing, slow, or sizeof(int) != sizeof(int32_t).
	 */
	static size_t outbuf_i = 0;
	static int32_t outbuf[BUFSIZ];
	outbuf[outbuf_i++] = x;
	if (outbuf_i == BUFSIZ) {
		fwrite(outbuf, sizeof(int32_t), outbuf_i, stdout);
		outbuf_i = 0;
	}
#endif
}
