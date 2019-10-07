#pragma once

#include "stdex/intrusive_ptr.h"

namespace stdex
{
    template<class T, class D = void>
    class intrusive_ptr_view
        : public intrusive_ptr<T, D>
    {
    public:
        typedef intrusive_ptr<T, D> base_type;
        using base_type::value_type;
        using base_type::derived_type;

        typedef intrusive_ptr<value_type, derived_type> value_ptr_type;

    public:
        intrusive_ptr_view()
            : base_type()
        {
        }

        intrusive_ptr_view( const intrusive_ptr_view & _rhs )
            : base_type( _rhs.m_ptr, intrusive_borrow_t() )
        {
        }

        intrusive_ptr_view( std::nullptr_t _ptr )
            : base_type()
        {
        }

        intrusive_ptr_view( const value_type * _ptr )
            : base_type( _ptr, intrusive_borrow_t() )
        {
        }

        ~intrusive_ptr_view()
        {
            base_type::m_ptr = nullptr;
        }

        intrusive_ptr_view & operator = ( const intrusive_ptr_view & _rhs ) noexcept
        {
            base_type::m_ptr = _rhs.get();

            return *this;
        }

        intrusive_ptr_view & operator = ( const value_type * _rhs ) noexcept
        {
            base_type::m_ptr = const_cast<value_type *>(_rhs);

            return *this;
        }

        intrusive_ptr_view & operator = ( std::nullptr_t ) noexcept
        {
            base_type::m_ptr = nullptr;

            return *this;
        }

    public:
        value_type * get() const
        {
            return m_ptr;
        }

        template<class U>
        U getT() const
        {
            return static_cast<U>(m_ptr);
        }

        value_type * operator -> () const
        {
            return m_ptr;
        }

        void reset()
        {
            m_ptr = nullptr;
        }

        void swap( intrusive_ptr_view & _rhs )
        {
            value_type * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    void swap( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D = void>
    intrusive_ptr_view<T, D> make_intrusive_ptr_view( const U * _iptr )
    {
        return intrusive_ptr_view<T, D>( _iptr );
    }    
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    U intrusive_ptr_view_get( const intrusive_ptr_view<T, D> & _iptr )
    {
        U * u_ptr = _iptr.getT<U>();

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator < ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator > ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator <= ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator >= ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<D> & _right )
    {
        const T * l = _left.get();
        const D * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr_view<T, D> & _left, const intrusive_ptr_view<D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr_view<D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        const D * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr_view<D> & _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator != ( const intrusive_ptr_view<T, D> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    bool operator != ( const U * _left, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = _right.get();

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator == ( const U * _left, const intrusive_ptr_view<T, D> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator == ( const intrusive_ptr_view<T, D> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr_view<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( std::nullptr_t, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = intrusive_ptr_view_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( std::nullptr_t, const intrusive_ptr_view<T, D> & _right )
    {
        const T * ptr = _right.get();

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr_view<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }    
}