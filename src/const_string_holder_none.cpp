#   include "stdex/const_string_holder_none.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const_string_holder_none::const_string_holder_none()
    {
        const char * empty_str = "";

        this->setup( empty_str, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder_none::_releaseString()
    {
        //Empty
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder_none::_destroyString()
    {
        //Empty
    }
}