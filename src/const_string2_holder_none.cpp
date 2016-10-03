#   include "stdex/const_string2_holder_none.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const_string2_holder_none::const_string2_holder_none()
    {
        const char * empty_str = "";

        this->setup( empty_str, 0, 0 );
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string2_holder_none::destroyString()
    {
        //Empty
    }
}