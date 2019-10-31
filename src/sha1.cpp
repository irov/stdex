#include "stdex/sha1.h"

namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    static inline unsigned int roll( const unsigned int value, const unsigned int steps )
    {
        return ((value << steps) | (value >> (32 - steps)));
    }
    //////////////////////////////////////////////////////////////////////////
    static inline void clear( unsigned int * buffert )
    {
        for( int pos = 16; --pos >= 0; )
        {
            buffert[pos] = 0;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    static void process( unsigned int * result, unsigned int * w )
    {
        unsigned int a = result[0];
        unsigned int b = result[1];
        unsigned int c = result[2];
        unsigned int d = result[3];
        unsigned int e = result[4];

        int round = 0;

#define abcde(func, value) \
						{ \
                const unsigned int t = roll(a, 5) + (func) + value + e + w[round]; \
				e = d; \
				d = c; \
				c = roll(b, 30); \
				b = a; \
				a = t; \
						}

        while( round < 16 )
        {
            abcde( (b & c) | (~b & d), 0x5a827999 )
                ++round;
        }
        while( round < 20 )
        {
            w[round] = roll( (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1 );
            abcde( (b & c) | (~b & d), 0x5a827999 )
                ++round;
        }
        while( round < 40 )
        {
            w[round] = roll( (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1 );
            abcde( b ^ c ^ d, 0x6ed9eba1 )
                ++round;
        }
        while( round < 60 )
        {
            w[round] = roll( (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1 );
            abcde( (b & c) | (b & d) | (c & d), 0x8f1bbcdc )
                ++round;
        }
        while( round < 80 )
        {
            w[round] = roll( (w[round - 3] ^ w[round - 8] ^ w[round - 14] ^ w[round - 16]), 1 );
            abcde( b ^ c ^ d, 0xca62c1d6 )
                ++round;
        }

#undef abcde

        result[0] += a;
        result[1] += b;
        result[2] += c;
        result[3] += d;
        result[4] += e;
    }
    //////////////////////////////////////////////////////////////////////////
    void sha1_calc( const void * src, const int bytelength, unsigned char * hash )
    {
        unsigned int result[5] = {0x67452301, 0xefcdab89, 0x98badcfe, 0x10325476, 0xc3d2e1f0};

        const unsigned char * sarray = (const unsigned char *)src;

        unsigned int w[80];

        const int endOfFullBlocks = bytelength - 64;
        int endCurrentBlock;
        int currentBlock = 0;

        while( currentBlock <= endOfFullBlocks )
        {
            endCurrentBlock = currentBlock + 64;

            for( int roundPos = 0; currentBlock < endCurrentBlock; currentBlock += 4 )
            {
                w[roundPos++] = (unsigned int)sarray[currentBlock + 3]
                    | (((unsigned int)sarray[currentBlock + 2]) << 8)
                    | (((unsigned int)sarray[currentBlock + 1]) << 16)
                    | (((unsigned int)sarray[currentBlock]) << 24);
            }

            process( result, w );
        }

        endCurrentBlock = bytelength - currentBlock;

        clear( w );

        int lastBlockBytes = 0;

        for( ; lastBlockBytes < endCurrentBlock; ++lastBlockBytes )
        {
            w[lastBlockBytes >> 2] |= (unsigned int)sarray[lastBlockBytes + currentBlock] << ((3 - (lastBlockBytes & 3)) << 3);
        }

        w[lastBlockBytes >> 2] |= 0x80 << ((3 - (lastBlockBytes & 3)) << 3);

        if( endCurrentBlock >= 56 )
        {
            process( result, w );
            clear( w );
        }

        w[15] = bytelength << 3;
        process( result, w );

        for( int hashByte = 20; --hashByte >= 0; )
        {
            hash[hashByte] = (result[hashByte >> 2] >> (((3 - hashByte) & 0x3) << 3)) & 0xff;
        }
    }

    void sha1_hex( const unsigned char * hash, char * hexstring )
    {
        const char hexDigits[] = {"0123456789abcdef"};

        for( int hashByte = 20; --hashByte >= 0; )
        {
            hexstring[hashByte << 1] = hexDigits[(hash[hashByte] >> 4) & 0xf];
            hexstring[(hashByte << 1) + 1] = hexDigits[hash[hashByte] & 0xf];
        }
        hexstring[40] = 0;
    }
}
