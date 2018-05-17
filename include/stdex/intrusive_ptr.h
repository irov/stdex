#pragma once

#include "stdex/exception.h"

#include <cstddef>

#ifndef STDEX_INTRUSIVE_PTR_DEBUG_ENABLE
#	ifndef NDEBUG
#	    define STDEX_INTRUSIVE_PTR_DEBUG
#	endif
#else
#	define STDEX_INTRUSIVE_PTR_DEBUG
#endif

#ifdef STDEX_INTRUSIVE_PTR_DEBUG
#	define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK() uint32_t m_debug_ptr_mask__;
#	define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK() this->m_debug_ptr_mask__ = 0xABCDEF01
#	define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK() if( this->m_debug_ptr_mask__ != 0xABCDEF01 ) STDEX_THROW_EXCEPTION("mask != 0xABCDEF01")
#else
#	define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
#	define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK()
#	define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK()
#endif

namespace stdex
{
    template<class T>
    class intrusive_ptr
    {
    public:
        typedef T element_type;

    public:
        inline static const intrusive_ptr & none()
        {
            static intrusive_ptr ptr_none;

            return ptr_none;
        }

    public:
        inline intrusive_ptr()
            : m_ptr( nullptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        inline intrusive_ptr( std::nullptr_t _ptr )
            : m_ptr( _ptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        inline intrusive_ptr( const intrusive_ptr & _rhs )
            : m_ptr( _rhs.get() )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        inline intrusive_ptr( intrusive_ptr && _rhs )
            : m_ptr( _rhs.get() )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.reset();
        }

        inline intrusive_ptr( element_type * _ptr )
            : m_ptr( _ptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U>
        inline intrusive_ptr( const intrusive_ptr<U> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U>
        inline intrusive_ptr( intrusive_ptr<U> && _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.reset();
        }

        template<class U>
        inline intrusive_ptr( const U * _ptr )
            : m_ptr( static_cast<element_type *>(const_cast<U *>(_ptr)) )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        inline ~intrusive_ptr()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            this->decref();
        }

        inline intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_ptr & operator = ( intrusive_ptr && _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_ptr & operator = ( element_type * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( const intrusive_ptr<U> & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( intrusive_ptr<U> && _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        inline intrusive_ptr & operator = ( U * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        inline intrusive_ptr & operator = ( std::nullptr_t )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        inline element_type * get() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            return m_ptr;
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
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
            if( m_ptr == nullptr )
            {
                STDEX_THROW_EXCEPTION( "m_ptr == nullptr" );
            }

            if( T::intrusive_ptr_check_ref( m_ptr ) == false )
            {
                STDEX_THROW_EXCEPTION( "ptr check == false" );
            }
#	endif

            return m_ptr;
        }

        inline void reset()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            m_ptr = nullptr;
        }


        inline void swap( intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            T * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }

    protected:
        inline void incref()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr != nullptr )
            {
                T::intrusive_ptr_add_ref( m_ptr );
            }
        }

        inline void decref()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr != nullptr )
            {
                T::intrusive_ptr_dec_ref( m_ptr );
            }
        }

    protected:
        element_type * m_ptr;

        STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void swap( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline intrusive_ptr<T> intrusive_from_this( T * _self )
    {
        return intrusive_ptr<T>( _self );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U intrusive_static_cast( const intrusive_ptr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = static_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U intrusive_dynamic_cast( const intrusive_ptr<T> & _iptr )
    {
        typedef typename U::element_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    inline U intrusive_get( const intrusive_ptr<T> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = static_cast<U>(t_ptr);

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator < ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator > ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator <= ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator >= ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator == ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator != ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    inline bool operator != ( const intrusive_ptr<T> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    inline bool operator != ( const U * _left, const intrusive_ptr<T> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    inline bool operator == ( const U * _left, const intrusive_ptr<T> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    inline bool operator == ( const intrusive_ptr<T> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator != ( const intrusive_ptr<T> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator != ( std::nullptr_t, const intrusive_ptr<T> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator == ( std::nullptr_t, const intrusive_ptr<T> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline bool operator == ( const intrusive_ptr<T> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }
}