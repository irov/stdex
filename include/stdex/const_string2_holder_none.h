#   pragma once

#   include "stdex/const_string2_holder.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    class const_string2_holder_none
        : public const_string2_holder
    {
    public:
        const_string2_holder_none();

    protected:
        void destroyString() override;

	private:
		void operator = ( const const_string2_holder_none & )
		{
		}
    };
}