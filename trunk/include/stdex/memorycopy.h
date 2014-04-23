#	pragma once

//#	include "stdex/unchecked_array_iterator.h"

#	include <algorithm>

namespace stdex
{	
	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
		//unchecked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist);
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist );
	}

	inline bool memorycopy_safe( void * _dist, size_t _offset, size_t _capacity, const void * _src, size_t _size )
	{
		if( _offset + _size > _capacity )
		{
			return false;
		}

		//unchecked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist + _offset);
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist + _offset );

		return true;
	}
}
