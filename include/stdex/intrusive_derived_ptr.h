#pragma once

#include "stdex/intrusive_ptr.h"

namespace stdex
{
    template<class T, class D>
    class intrusive_derived_ptr
        : public intrusive_ptr<D>
    {
    public:
        typedef T element_type;
        typedef typename intrusive_ptr<D>::element_type derived_type;

    public:
        inline static const intrusive_derived_ptr & none()
        {
            static intrusive_derived_ptr ptr_none;

            return ptr_none;
        }

    public:
        inline intrusive_derived_ptr()
            : intrusive_ptr<derived_type>( nullptr )
        {
        }

        inline intrusive_derived_ptr( const intrusive_derived_ptr & _rhs )
            : intrusive_ptr<derived_type>( _rhs.m_ptr )
        {
        }

        inline intrusive_derived_ptr( intrusive_derived_ptr && _rhs )
            : intrusive_ptr<derived_type>( std::move( _rhs.m_ptr ) )
        {
        }

        inline intrusive_derived_ptr( std::nullptr_t _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
        }

        inline intrusive_derived_ptr( const intrusive_ptr<derived_type> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
        }

        inline intrusive_derived_ptr( intrusive_ptr<derived_type> && _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
        }

        inline intrusive_derived_ptr( element_type * _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
        }

        template<class U, class UD>
        inline intrusive_derived_ptr( const intrusive_derived_ptr<U, UD> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
        }

        template<class U, class UD>
        inline intrusive_derived_ptr( intrusive_derived_ptr<U, UD> && _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
        }

        template<class U>
        inline intrusive_derived_ptr( const U * _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
        }

        inline ~intrusive_derived_ptr()
        {
        }

        inline intrusive_derived_ptr & operator = ( const intrusive_derived_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_derived_ptr & operator = ( intrusive_derived_ptr && _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_derived_ptr & operator = ( element_type * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        intrusive_derived_ptr & operator = ( const intrusive_derived_ptr<U, UD> & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        intrusive_derived_ptr & operator = ( intrusive_derived_ptr<U, UD> && _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        inline intrusive_derived_ptr & operator = ( U * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_derived_ptr & operator = ( std::nullptr_t )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_derived_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        inline element_type * get() const
        {
            element_type * ptr = intrusive_ptr<derived_type>::template getT<element_type *>();

            return ptr;
        }

        template<class K>
        inline K getT() const
        {
            element_type * ptr = this->get();

            K ptr_t = static_cast<K>(ptr);

            return ptr_t;
        }

        inline element_type * operator -> () const
        {
            derived_type * ptr = intrusive_ptr<derived_type>::operator ->();

            return static_cast<element_type *>(ptr);
        }

        inline void reset()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr<derived_type>::m_ptr = nullptr;
        }

        inline void swap( intrusive_derived_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            derived_type * tmp = intrusive_ptr<derived_type>::m_ptr;
            intrusive_ptr<derived_type>::m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline void swap( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline U intrusive_static_cast( const intrusive_derived_ptr<T, D> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = static_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline U intrusive_dynamic_cast( const intrusive_derived_ptr<T, D> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline U intrusive_get( const intrusive_derived_ptr<T, D> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = static_cast<U>(t_ptr);

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator < ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator > ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator <= ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator >= ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_derived_ptr<T, D> & _left, const intrusive_derived_ptr<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator != ( const intrusive_derived_ptr<T, D> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    inline bool operator != ( const U * _left, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator == ( const U * _left, const intrusive_derived_ptr<T, D> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    inline bool operator == ( const intrusive_derived_ptr<T, D> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( const intrusive_derived_ptr<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator != ( std::nullptr_t, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( std::nullptr_t, const intrusive_derived_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    inline bool operator == ( const intrusive_derived_ptr<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }
}