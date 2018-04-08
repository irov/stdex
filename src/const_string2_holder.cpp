#	include "stdex/const_string2_holder.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const_string2_holder::const_string2_holder()
        : m_data( nullptr )
        , m_hash( 0 )
        , m_size( 0 )
    {
        //stdex::intrusive_this_aquire( this );
    }
    //////////////////////////////////////////////////////////////////////////
    const_string2_holder::const_string2_holder( const const_string2_holder & _holder )
        : m_data( _holder.m_data )
        , m_size( _holder.m_size )
        , m_hash( _holder.m_hash )
    {
    }
    //////////////////////////////////////////////////////////////////////////
	void const_string2_holder::setup( const char * _data, size_type _size, hash_type _hash )
    {
        m_data = _data;
		m_size = _size;

        m_hash = _hash;
    }
}