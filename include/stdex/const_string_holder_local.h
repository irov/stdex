#   pragma once

#   include "stdex/const_string_holder.h"

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    class const_string_holder_local
        : public const_string_holder
    {
	public:
		const_string_holder_local();
		const_string_holder_local( const char * _data, size_t _size, hash_type _hash );

	private:
		void operator = ( const const_string_holder_local & )
		{
		}

	public:
		void setup( const char * _data, size_t _size, hash_type _hash );

    protected:
        void _releaseString() override;
        void _destroyString() override;
    };
}