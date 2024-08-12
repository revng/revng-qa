// These functions were copied from the musl implementation.
// The license:
//
// ----------------------------------------------------------------------
// Copyright © 2005-2020 Rich Felker, et al.
// 
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
// 
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
// IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
// CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
// TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
// SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
// ----------------------------------------------------------------------

#include <string.h>
#include <stdint.h>
#include <endian.h>

int memcmp(const void *vl, const void *vr, size_t n)
{
	const unsigned char *l=vl, *r=vr;
	for (; n && *l == *r; n--, l++, r++);
	return n ? *l-*r : 0;
}

void *memset(void *dest, int c, size_t n)
{
	unsigned char *s = dest;
	size_t k;

	/* Fill head and tail with minimal branching. Each
	 * conditional ensures that all the subsequently used
	 * offsets are well-defined and in the dest region. */

	if (!n) return dest;
	s[0] = s[n-1] = c;
	if (n <= 2) return dest;
	s[1] = s[n-2] = c;
	s[2] = s[n-3] = c;
	if (n <= 6) return dest;
	s[3] = s[n-4] = c;
	if (n <= 8) return dest;

	/* Advance pointer to align it at a 4-byte boundary,
	 * and truncate n to a multiple of 4. The previous code
	 * already took care of any head/tail that get cut off
	 * by the alignment. */

	k = -(uintptr_t)s & 3;
	s += k;
	n -= k;
	n &= -4;

	/* Pure C fallback with no aliasing violations. */
	for (; n; n--, s++) *s = c;

	return dest;
}

void *memcpy(void *restrict dest, const void *restrict src, size_t n)
{
	unsigned char *d = dest;
	const unsigned char *s = src;

	for (; n; n--) *d++ = *s++;
	return dest;
}

void *memmove(void *dest, const void *src, size_t n)
{
  return memcpy(dest, src, n);
}
