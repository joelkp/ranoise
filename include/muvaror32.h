/* muvaror32 reference code & common header for test programs using it
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

/** 32-bit Fibonacci hash constant. */
#define FIBH32 2654435769UL

/** Rotate bits right, for 32-bit unsigned \p x value, \p r length. */
#define ROR32(x, r) \
	((uint32_t)(x) >> ((r) & 31) | (uint32_t)(x) << ((32-(r)) & 31))

/** Multiplicatively mix bits using varying right-rotation,
    for 32-bit unsigned \p x value, \p r rotation, \p ro rotation offset. */
#define MUVAROR32(x, r, ro) \
	(((uint32_t)(x) | ((1<<((ro) & 31))|1)) * ROR32((x), (r)+(ro)))
