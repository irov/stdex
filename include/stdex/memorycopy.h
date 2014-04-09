#	pragma once

#	pragma warning( disable : 4996 )

#	include <algorithm>
#	include <iterator>

namespace stdex
{	
	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
		//stdext::unchecked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist);
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist );
	}
}

#	pragma warning( default : 4996 )