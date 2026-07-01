#pragma once

#include "stdex/allocator.h"

#include <cstddef>
#include <limits>
#include <memory>
#include <new>

namespace stdex
{
    template<class T>
    class stl_allocator
    {
    public:
        typedef T value_type;

        stl_allocator() = default;

        template<class U>
        stl_allocator( const stl_allocator<U> & ) noexcept
        {
        }

        T * allocate( std::size_t _count, const void * = nullptr )
        {
            void * mem = stdex_malloc( sizeof( T ) * _count );

            return static_cast<T *>(mem);
        }

        void deallocate( T * _ptr, std::size_t )
        {
            stdex_free( _ptr );
        }
    };

    template<class T>
    bool operator == ( const stl_allocator<T> &, const stl_allocator<T> & )
    {
        return true;
    }

    template<class T>
    bool operator != ( const stl_allocator<T> &, const stl_allocator<T> & )
    {
        return false;
    }
}
