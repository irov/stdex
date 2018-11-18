#pragma once

#include "stdex/intrusive_ptr.h"

namespace stdex
{
    template<class T, class D = void>
    class intrusive_ptr_view
    {
    public:
        typedef T value_type;
        typedef D derived_type;
        typedef intrusive_ptr<value_type, derived_type> value_ptr_type;

    public:
        inline intrusive_ptr_view()
            : m_ptr( nullptr )
        {
        }

        inline intrusive_ptr_view( const intrusive_ptr_view & _rhs )
            : m_ptr( _rhs.m_ptr )
        {
        }

        inline intrusive_ptr_view( std::nullptr_t _ptr )
            : m_ptr( nullptr )
        {
        }

        inline intrusive_ptr_view( value_type * _ptr )
            : m_ptr( _ptr )
        {
        }

        template<class U, class UD>
        inline intrusive_ptr_view( const intrusive_ptr<U, UD> & _rhs )
            : m_ptr( _rhs.get() )
        {
        }

        template<class U>
        inline intrusive_ptr_view( const U * _ptr )
            : m_ptr( static_cast<value_type *>(_ptr) )
        {
        }

        inline ~intrusive_ptr()
        {
        }

        inline intrusive_ptr_view & operator = ( const intrusive_ptr_view & _rhs ) noexcept
        {
            m_ptr = _rhs.get();

            return *this;
        }

        inline intrusive_ptr_view & operator = ( value_type * _rhs ) noexcept
        {
            m_ptr = _rhs;

            return *this;
        }

        template<class U, class UD>
        intrusive_ptr_view & operator = ( const intrusive_ptr<U, UD> & _rhs ) noexcept
        {
            m_ptr = _rhs.getT<value_type>();

            return *this;
        }

        template<class U>
        inline intrusive_ptr_view & operator = ( U * _rhs ) noexcept
        {
            m_ptr = static_cast<value_type *>(_rhs);

            return *this;
        }

        inline intrusive_ptr_view & operator = ( std::nullptr_t ) noexcept
        {
            m_ptr = nullptr;

            return *this;
        }

    public:
        inline value_type * get() const
        {
            return m_ptr;
        }

        template<class U>
        inline U getT() const
        {
            return static_cast<U>(m_ptr);
        }

        inline value_type * operator -> () const
        {
            return m_ptr;
        }

        inline void reset()
        {
            m_ptr = nullptr;
        }

        inline void swap( intrusive_ptr_view & _rhs )
        {
            value_type * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline void swap( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D = void>
    inline intrusive_ptr_view<T, D> make_intrusive_ptr_view( const U * _iptr )
    {
        return intrusive_ptr_view<T, D>( _iptr );
    }    
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline U intrusive_ptr_view_get( const intrusive_ptr_view<T, D> & _iptr )
    {
        U * u_ptr = _iptr.getT<U>();

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator < ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator > ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator <= ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator >= ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<D> & _right )
    {
        const T * l = _left.get();
        const D * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_ptr_view<D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const D * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_ptr_view<D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator != ( const intrusive_ptr_view<T, D> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline bool operator != ( const U * _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = _right.get();

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator == ( const U * _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator == ( const intrusive_ptr_view<T, D> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_ptr_view<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( std::nullptr_t, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = intrusive_ptr_view_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( std::nullptr_t, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = _right.get();

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_ptr<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }    
}