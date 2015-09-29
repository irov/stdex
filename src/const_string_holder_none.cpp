#   include "stdex/const_string_holder_none.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    const_string_holder_none::const_string_holder_none()
    {
		STDEX_THREAD_GUARD_RESET(this);

        const char * empty_str = "";

        this->setup( empty_str, 0, 0, false );
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