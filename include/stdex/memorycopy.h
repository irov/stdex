#pragma once

#include <stddef.h>

namespace stdex
{
    namespace detail
    {
        void memorycopy_impl( void * _dist, size_t _offset, const void * _src, size_t _size );
    }
    //////////////////////////////////////////////////////////////////////////
    inline void memorycopy( void * _dist, size_t _offset, const void * _src, size_t _size )
    {
        detail::memorycopy_impl( _dist, _offset, _src, _size );
    }
    //////////////////////////////////////////////////////////////////////////
    inline bool memorycopy_safe( void * _dist, size_t _offset, size_t _capacity, const void * _src, size_t _size )
    {
        if( _offset + _size > _capacity )
        {
            return false;
        }

        detail::memorycopy_impl( _dist, _offset, _src, _size );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void memorycopy_pod( T * _dist, size_t _offset, const T * _src, size_t _size )
    {
        detail::memorycopy_impl( _dist, _offset * sizeof( T ), _src, _size * sizeof( T ) );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool memorycopy_safe_pod( T * _dist, size_t _offset, size_t _capacity, const T * _src, size_t _size )
    {
        if( _offset + _size > _capacity )
        {
            return false;
        }

        detail::memorycopy_impl( _dist, _offset * sizeof( T ), _src, _size * sizeof( T ) );

        return true;
    }
}
