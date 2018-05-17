#pragma once

namespace stdex
{
    void sha1_calc(const void* src, const int bytelength, unsigned char* hash);
	void sha1_hex( const unsigned char * hash, char * hexstring );
}
