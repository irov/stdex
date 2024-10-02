#pragma once

namespace stdex
{
    void sha1_calc( const void * src, const int bytelength, unsigned char * hash );
    void sha1_hex( const unsigned char * hash, char * const hexstring );
    void sha1_hexw( const unsigned char * hash, wchar_t * const hexstring );
}
