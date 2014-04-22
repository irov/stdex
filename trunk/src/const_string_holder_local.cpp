#   include "stdex/const_string_holder_local.h"

namespace stdex
{    
	//////////////////////////////////////////////////////////////////////////
	const_string_holder_local::const_string_holder_local()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	const_string_holder_local::const_string_holder_local( const char * _data, size_t _size )
	{
		const_string_holder::setup( _data, _size, false );
	}
	//////////////////////////////////////////////////////////////////////////
	void const_string_holder_local::setup( const char * _data, size_t _size )
	{
		const_string_holder::setup( _data, _size, false );
	}
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder_local::_releaseString()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder_local::_destroyString()
    {
        //Empty
    }
}