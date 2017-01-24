#ifndef CRYPTOPP_WORDS_H
#define CRYPTOPP_WORDS_H

#include "misc.h"

NAMESPACE_BEGIN(CryptoPP)

inline uint32_t CountWords(const word *X, uint32_t N)
{
	while (N && X[N-1]==0)
		N--;
	return N;
}

inline void SetWords(word *r, word a, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] = a;
}

inline void CopyWords(word *r, const word *a, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] = a[i];
}

inline void XorWords(word *r, const word *a, const word *b, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] = a[i] ^ b[i];
}

inline void XorWords(word *r, const word *a, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] ^= a[i];
}

inline void AndWords(word *r, const word *a, const word *b, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] = a[i] & b[i];
}

inline void AndWords(word *r, const word *a, uint32_t n)
{
	for (uint32_t i=0; i<n; i++)
		r[i] &= a[i];
}

inline word ShiftWordsLeftByBits(word *r, uint32_t n, uint32_t shiftBits)
{
	assert (shiftBits<WORD_BITS);
	word u, carry=0;
	if (shiftBits)
		for (uint32_t i=0; i<n; i++)
		{
			u = r[i];
			r[i] = (u << shiftBits) | carry;
			carry = u >> (WORD_BITS-shiftBits);
		}
	return carry;
}

inline word ShiftWordsRightByBits(word *r, uint32_t n, uint32_t shiftBits)
{
	assert (shiftBits<WORD_BITS);
	word u, carry=0;
	if (shiftBits)
		for (int i=n-1; i>=0; i--)
		{
			u = r[i];
			r[i] = (u >> shiftBits) | carry;
			carry = u << (WORD_BITS-shiftBits);
		}
	return carry;
}

inline void ShiftWordsLeftByWords(word *r, uint32_t n, uint32_t shiftWords)
{
	shiftWords = STDMIN(shiftWords, n);
	if (shiftWords)
	{
		for (uint32_t i=n-1; i>=shiftWords; i--)
			r[i] = r[i-shiftWords];
		SetWords(r, 0, shiftWords);
	}
}

inline void ShiftWordsRightByWords(word *r, uint32_t n, uint32_t shiftWords)
{
	shiftWords = STDMIN(shiftWords, n);
	if (shiftWords)
	{
		for (uint32_t i=0; i+shiftWords<n; i++)
			r[i] = r[i+shiftWords];
		SetWords(r+n-shiftWords, 0, shiftWords);
	}
}

NAMESPACE_END

#endif
