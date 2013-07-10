#	include "stdex/const_string.h"

#   include <string.h>

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const const_string & const_string::none()
    {
        static const_string s_none( new const_string_holder_none() );

        return s_none;
    }
    //////////////////////////////////////////////////////////////////////////
    bool operator == ( const const_string & _left, const char * _right )
    {
        const char * left_str = _left.c_str();

        if( strcmp( left_str, _right ) != 0 )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    bool operator == ( const char * _left, const const_string & _right )
    {
        const char * right_str = _right.c_str();

        if( strcmp( _left, right_str ) != 0 )
        {
            return false;
        }

        return true;
    }
}