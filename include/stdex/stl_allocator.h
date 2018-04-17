#pragma once

#include <stdex/allocator.h>

#include <new>
#include <limits>
#include <memory>
#include <type_traits>

namespace stdex
{
    template <typename T>
    class stl_allocator
    {
    public:
        typedef T value_type;
       
        stl_allocator() = default;              
        template <class U> stl_allocator( const stl_allocator<U>& ) noexcept {}

        T * allocate( std::size_t n, const void * = nullptr )
        {
            return static_cast<T *>(malloc( n * sizeof( T ) ));
        }

        void deallocate( T * p, std::size_t )
        {
            free( p );
        }

        //template<class U, class... Args>
        //void construct( U * p, Args&&... _args )
        //{
        //    ::new (static_cast<void *>(_Ptr)) U( std::forward<Args>( _args )... );
        //}
        //
        //template<class U>
        //void destroy( U * p )
        //{
        //    p->~U();
        //}
    };

    template <class T>
    bool operator == ( const stl_allocator<T>&, const stl_allocator<T>& )
    {
        return true;
    }

    template <class T>
    bool operator != ( const stl_allocator<T>&, const stl_allocator<T>& )
    {
        return false;
    }
}