#	include "stdex/const_string2_holder.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
	void const_string2_holder::setup( const char * _data, size_type _size, hash_type _hash )
    {
        m_data = _data;
		m_size = _size;

        m_hash = _hash;
    }
}