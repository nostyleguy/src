// misc.cpp - written and placed in the public domain by Wei Dai

#include "FirstCrypto.h"
#include "misc.h"
#include "words.h"

NAMESPACE_BEGIN(CryptoPP)

byte OAEP_P_DEFAULT[1];

void xorbuf(byte *buf, const byte *mask, unsigned long count)
{
	if (((unsigned long)buf | (unsigned long)mask | count) % WORD_SIZE == 0)
		XorWords((word *)buf, (const word *)mask, count/WORD_SIZE);
	else
	{
		for (unsigned long i=0; i<count; i++)
			buf[i] ^= mask[i];
	}
}

void xorbuf(byte *output, const byte *input, const byte *mask, unsigned long count)
{
	if (((unsigned long)output | (unsigned long)input | (unsigned long)mask | count) % WORD_SIZE == 0)
		XorWords((word *)output, (const word *)input, (const word *)mask, count/WORD_SIZE);
	else
	{
		for (unsigned long i=0; i<count; i++)
			output[i] = input[i] ^ mask[i];
	}
}

unsigned long Parity(unsigned long value)
{
	for (unsigned long i=8*sizeof(value)/2; i>0; i/=2)
		value ^= value >> i;
	return (unsigned long)value&1;
}

unsigned long BytePrecision(unsigned long value)
{
	unsigned long i;
	for (i=sizeof(value); i; --i)
		if (value >> (i-1)*8)
			break;

	return i;
}

unsigned long BitPrecision(unsigned long value)
{
	if (!value)
		return 0;

	unsigned long l=0, h=8*sizeof(value);

	while (h-l > 1)
	{
		unsigned long t = (l+h)/2;
		if (value >> t)
			l = t;
		else
			h = t;
	}

	return h;
}

unsigned long Crop(unsigned long value, unsigned long size)
{
	if (size < 8*sizeof(value))
    	return (value & ((1L << size) - 1));
	else
		return value;
}

NAMESPACE_END
