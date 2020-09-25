#pragma once

#include <stdex/allocator.h>

#include <new>
#include <limits>
#include <memory>

namespace stdex
{
    template <typename T>
    class stl_allocator
    {
    public:
        typedef T value_type;

        stl_allocator() = default;
        template <class U> stl_allocator( const stl_allocator<U> & ) noexcept
        {
        }

        T * allocate( std::size_t n, const void * = nullptr )
        {
            const std::size_t element_size = sizeof( T );
            const std::size_t total_size = element_size * n;
            void * p = stdex_malloc( total_size );
            return static_cast<T *>(p);
        }

        void deallocate( T * p, std::size_t )
        {
            stdex_free( p );
        }
    };

    template <class T>
    bool operator == ( const stl_allocator<T> &, const stl_allocator<T> & )
    {
        return true;
    }

    template <class T>
    bool operator != ( const stl_allocator<T> &, const stl_allocator<T> & )
    {
        return false;
    }
}