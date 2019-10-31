#include "stdex/memorycopy.h"

#include <algorithm>

//#include "stdex/unchecked_array_iterator.h"

namespace stdex
{
    namespace detail
    {
        //////////////////////////////////////////////////////////////////////////	
        void memorycopy_impl( void * _dist, size_t _offset, const void * _src, size_t _size )
        {
            //std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, stdex::unchecked_array_iterator<unsigned char *>((unsigned char *)_dist + _offset) );
            std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, (unsigned char *)_dist + _offset );
        }
    }
}