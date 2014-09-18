#   pragma once

#   include "stdex/const_string_holder.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    class const_string_holder_none
        : public const_string_holder
    {
    public:
        const_string_holder_none();

	private:
		void operator = ( const const_string_holder_none & )
		{
		}

    protected:
        void _releaseString() override;
        void _destroyString() override;
    };
}