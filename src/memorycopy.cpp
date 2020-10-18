#include "stdex/memorycopy.h"

#include <algorithm>

#include <stdint.h>

//#include "stdex/unchecked_array_iterator.h"

namespace stdex
{
    namespace detail
    {
        //////////////////////////////////////////////////////////////////////////	
        void memorycopy_impl( void * _dist, size_t _offset, const void * _src, size_t _size )
        {
            //std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset) );
            std::copy( (uint8_t *)_src, (uint8_t *)_src + _size, (uint8_t *)_dist + _offset );
        }
    }
}