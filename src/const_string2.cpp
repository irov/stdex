#	include "stdex/const_string2.h"

#   include <string.h>

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const const_string2 & const_string2::none()
    {
        static const_string2 s_none( new const_string2_holder_none() );

        return s_none;
    }
    //////////////////////////////////////////////////////////////////////////
    bool operator == ( const const_string2 & _left, const const_string2::value_type * _right )
    {
        const const_string2::value_type * left_str = _left.c_str();

        if( strcmp( left_str, _right ) != 0 )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool operator == ( const const_string2::value_type * _left, const const_string2 & _right )
    {
        const const_string2::value_type * right_str = _right.c_str();

        if( strcmp( _left, right_str ) != 0 )
        {
            return false;
        }

        return true;
    }
	//////////////////////////////////////////////////////////////////////////
	bool operator != ( const const_string2 & _left, const const_string2::value_type * _right )
	{
		const const_string2::value_type * left_str = _left.c_str();

		if( strcmp( left_str, _right ) == 0 )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool operator != ( const const_string2::value_type * _left, const const_string2 & _right )
	{
		const const_string2::value_type * right_str = _right.c_str();

		if( strcmp( _left, right_str ) == 0 )
		{
			return false;
		}

		return true;
	}
}