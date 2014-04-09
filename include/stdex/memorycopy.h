#	pragma once

#	include <algorithm>
#	include <iterator>

namespace stdex
{	
	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
		stdext::checked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist, _size );
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, chkd_dist );
	}
}