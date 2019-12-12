#pragma once

#include "stdex/exception.h"

#include <cstddef>

#ifndef STDEX_INTRUSIVE_PTR_DEBUG_ENABLE
#   ifndef NDEBUG
#       define STDEX_INTRUSIVE_PTR_DEBUG
#   endif
#else
#   define STDEX_INTRUSIVE_PTR_DEBUG
#endif

#if defined(WIN32) && !defined(NDEBUG)

#pragma warning(disable:6011)

inline void stdex_critical_crash_error()
{
    volatile unsigned int * p = nullptr; *p = 0xBADF00D;
}

#pragma warning(default:6011)

#define STDEX_CRITICAL_CRASH_ERROR stdex_critical_crash_error()
#else
#define STDEX_CRITICAL_CRASH_ERROR 
#endif

#ifdef STDEX_INTRUSIVE_PTR_DEBUG
template<class V, class T>
void stdex_intrusive_ptr_check_typecast_ptr( T _ptr )
{
    if( _ptr != nullptr && dynamic_cast<V>(_ptr) == nullptr )
    {
        STDEX_CRITICAL_CRASH_ERROR;
        STDEX_THROW_EXCEPTION( "ptr invalid cast" );
    }
}

template<class T>
void stdex_intrusive_ptr_check_debug_mask( T _ptr )
{
    if( _ptr->m_debug_ptr_mask__ != 0xABCDEF01 )
    {
        STDEX_CRITICAL_CRASH_ERROR;
        STDEX_THROW_EXCEPTION( "mask != 0xABCDEF01" );
    }
}

#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE) stdex_intrusive_ptr_check_typecast_ptr<TYPE>( PTR )
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK() public: uint32_t m_debug_ptr_mask__; protected:
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK() this->m_debug_ptr_mask__ = 0xABCDEF01
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK() stdex_intrusive_ptr_check_debug_mask( this )
#else
#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE)
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK()
#endif

namespace stdex
{
    struct intrusive_borrow_t
    {
    };

    template<class T, class D = void>
    class intrusive_ptr
        : public intrusive_ptr<D, void>
    {
    public:
        typedef T value_type;
        typedef D derived_type;
        typedef T * pointer_type;
        typedef const T * const_pointer_type;
        typedef intrusive_ptr<derived_type, void> derived_type_ptr;

    public:
        static const intrusive_ptr & none()
        {
            static intrusive_ptr ptr_none;

            return ptr_none;
        }

        static intrusive_ptr from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U, class UD>
        static intrusive_ptr from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.template getT<pointer_type>() );
        }

        template<class U, class UD>
        static pointer_type ptr( const intrusive_ptr<U, UD> & _pointer )
        {
            return _pointer.template getT<pointer_type>();
        }

    public:
        intrusive_ptr()
            : intrusive_ptr<derived_type>( nullptr )
        {
        }

        intrusive_ptr( const intrusive_ptr & _rhs )
            : intrusive_ptr<derived_type>( _rhs.m_ptr )
        {
        }

        intrusive_ptr( intrusive_ptr && _rhs ) noexcept
            : intrusive_ptr<derived_type>( std::forward<intrusive_ptr &&>( _rhs ) )
        {
        }

        intrusive_ptr( std::nullptr_t _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
        }

        intrusive_ptr( const derived_type_ptr & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        intrusive_ptr( derived_type_ptr && _rhs )
            : derived_type_ptr( std::forward<derived_type_ptr &&>( _rhs ) )
        {
        }

        explicit intrusive_ptr( const value_type * _ptr )
            : derived_type_ptr( _ptr )
        {
        }

        explicit intrusive_ptr( const value_type * _ptr, intrusive_borrow_t )
            : derived_type_ptr( _ptr, intrusive_borrow_t() )
        {
        }

        template<class U>
        intrusive_ptr( const intrusive_ptr<U> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        template<class U>
        intrusive_ptr( intrusive_ptr<U> && _rhs )
            : intrusive_ptr<derived_type>( std::forward<intrusive_ptr<U> &&>( _rhs ) )
        {
        }

        template<class U, class UD>
        intrusive_ptr( const intrusive_ptr<U, UD> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        template<class U, class UD>
        intrusive_ptr( intrusive_ptr<U, UD> && _rhs )
            : intrusive_ptr<derived_type>( std::forward<intrusive_ptr<U, UD> &&>( _rhs ) )
        {
        }

        template<class U>
        explicit intrusive_ptr( const U * _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, pointer_type );
        }

        ~intrusive_ptr()
        {
        }

        intrusive_ptr & operator = ( const intrusive_ptr & _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( intrusive_ptr && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( value_type * _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        intrusive_ptr & operator = ( const intrusive_ptr<U, UD> & _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        intrusive_ptr & operator = ( intrusive_ptr<U, UD> && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( U * _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( std::nullptr_t ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        value_type * get() const
        {
            value_type * ptr = intrusive_ptr<derived_type>::template getT<value_type *>();

            return ptr;
        }

        template<class K>
        K getT() const
        {
            value_type * ptr = this->get();

            K ptr_t = static_cast<K>(ptr);

            return ptr_t;
        }

        value_type * move()
        {
            derived_type * ptr = intrusive_ptr<derived_type>::move();

            return static_cast<value_type *>(ptr);
        }

        template<class U, class UD>
        intrusive_ptr<U, UD> && moveT()
        {
            value_type * ptr = this->move();

            U * ptr_t = static_cast<U *>(ptr);

            return std::move( intrusive_ptr<U, UD>( ptr_t, intrusive_borrow_t() ) );
        }

        value_type * operator -> () const
        {
            derived_type * ptr = intrusive_ptr<derived_type>::operator ->();

            return static_cast<value_type *>(ptr);
        }

        void reset()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr<derived_type>::m_ptr = nullptr;
        }

        void swap( intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            derived_type * tmp = intrusive_ptr<derived_type>::m_ptr;
            intrusive_ptr<derived_type>::m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    void swap( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    T make_intrusive_ptr( const U * _iptr )
    {
        typedef typename T::value_type value_type;

        return T( static_cast<const value_type *>(_iptr) );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    U intrusive_static_cast( const intrusive_ptr<T, D> & _iptr )
    {
        typedef typename U::value_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = static_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    U intrusive_dynamic_cast( const intrusive_ptr<T, D> & _iptr )
    {
        typedef typename U::value_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    U intrusive_get( const intrusive_ptr<T, D> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = static_cast<U>(t_ptr);

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator < ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator > ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator <= ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator >= ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<D> & _right )
    {
        const T * l = _left.get();
        const D * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr<T, D> & _left, const intrusive_ptr<D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr<D> & _left, const intrusive_ptr<T, D> & _right )
    {
        const D * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr<D> & _left, const intrusive_ptr<T, D> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator != ( const intrusive_ptr<T, D> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    bool operator != ( const U * _left, const intrusive_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator == ( const U * _left, const intrusive_ptr<T, D> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D, class U>
    bool operator == ( const intrusive_ptr<T, D> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( const intrusive_ptr<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator != ( std::nullptr_t, const intrusive_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( std::nullptr_t, const intrusive_ptr<T, D> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    bool operator == ( const intrusive_ptr<T, D> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    class intrusive_ptr<T, void>
    {
    public:
        typedef T value_type;
        typedef void derived_type;
        typedef T * pointer_type;
        typedef const T * const_pointer_type;

    public:
        static const intrusive_ptr & none()
        {
            static intrusive_ptr ptr_none;

            return ptr_none;
        }

        static intrusive_ptr from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U, class UD>
        static intrusive_ptr from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.get() );
        }

        template<class U, class UD>
        static pointer_type ptr( const intrusive_ptr<U, UD> & _pointer )
        {
            return _pointer.template getT<pointer_type>();
        }

    public:
        intrusive_ptr() noexcept
            : m_ptr( nullptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        intrusive_ptr( std::nullptr_t _ptr )
            : m_ptr( _ptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        intrusive_ptr( const intrusive_ptr & _rhs )
            : m_ptr( _rhs.get() )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        intrusive_ptr( intrusive_ptr && _rhs ) noexcept
            : m_ptr( _rhs.get() )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.reset();
        }

        explicit intrusive_ptr( const value_type * _ptr )
            : m_ptr( const_cast<pointer_type>(_ptr) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, const_pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        explicit intrusive_ptr( const value_type * _ptr, intrusive_borrow_t )
            : m_ptr( const_cast<pointer_type>(_ptr) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, const_pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        template<class U>
        intrusive_ptr( const intrusive_ptr<U, void> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U>
        intrusive_ptr( intrusive_ptr<U, void> && _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.reset();
        }

        template<class U, class Y>
        intrusive_ptr( const intrusive_ptr<U, Y> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U, class Y>
        intrusive_ptr( intrusive_ptr<U, Y> && _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.reset();
        }

        template<class U>
        explicit intrusive_ptr( const U * _ptr )
            : m_ptr( static_cast<value_type *>(const_cast<U *>(_ptr)) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, const_pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        ~intrusive_ptr()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            this->decref();
        }

        intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( intrusive_ptr && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( value_type * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( const intrusive_ptr<U, void> & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( intrusive_ptr<U, void> && _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        intrusive_ptr & operator = ( U * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( std::nullptr_t )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        void set( value_type * _ptr )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            m_ptr = _ptr;
        }

        value_type * get() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            return m_ptr;
        }

        template<class K>
        K getT() const
        {
            value_type * ptr = this->get();

            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( ptr, K );

            K ptr_t = static_cast<K>(ptr);

            return ptr_t;
        }

        value_type * move()
        {
            value_type * ptr = m_ptr;
            m_ptr = nullptr;

            return ptr;
        }

        template<class U, class D>
        intrusive_ptr<U, D> moveT()
        {
            value_type * ptr = this->move();

            U * ptr_t = static_cast<U *>(ptr);

            return intrusive_ptr<U, D>( ptr_t, intrusive_borrow_t() );
        }


        value_type * operator -> () const
        {
#   ifdef STDEX_INTRUSIVE_PTR_DEBUG
            if( m_ptr == nullptr )
            {
                STDEX_CRITICAL_CRASH_ERROR;
                STDEX_THROW_EXCEPTION( "m_ptr == nullptr" );
            }

            if( T::intrusive_ptr_check_ref( m_ptr ) == false )
            {
                STDEX_CRITICAL_CRASH_ERROR;
                STDEX_THROW_EXCEPTION( "ptr check == false" );
            }
#   endif

            return m_ptr;
        }

        void reset()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            m_ptr = nullptr;
        }


        void swap( intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            T * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }

    protected:
        void incref()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr != nullptr )
            {
                T::intrusive_ptr_add_ref( m_ptr );
            }
        }

        void decref()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr != nullptr )
            {
                T::intrusive_ptr_dec_ref( m_ptr );
            }
        }

    protected:
        value_type * m_ptr;

        STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void swap( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        _left.swap( _right );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    intrusive_ptr<T, void> intrusive_from_this( T * _self )
    {
        return intrusive_ptr<T, void>( _self );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_reinterpret_cast( const intrusive_ptr<T, void> & _iptr )
    {
        typedef typename U::pointer_type U_type;

        T * t_ptr = _iptr.get();
        U_type u_ptr = static_cast<U_type>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_static_cast( const intrusive_ptr<T, void> & _iptr )
    {
        typedef typename U::pointer_type U_type;

        T * t_ptr = _iptr.get();
        U_type u_ptr = static_cast<U_type>(t_ptr);

        STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( t_ptr, U_type );

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_dynamic_cast( const intrusive_ptr<T, void> & _iptr )
    {
        typedef typename U::value_type U_type;

        T * t_ptr = _iptr.get();
        U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

        return U( u_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T>
    U intrusive_get( const intrusive_ptr<T, void> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = static_cast<U>(t_ptr);

        STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( t_ptr, U );

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator < ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l < r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator > ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l > r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator <= ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l <= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator >= ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l >= r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        const T * l = _left.get();
        const T * r = _right.get();

        return l == r;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( const intrusive_ptr<T, void> & _left, const intrusive_ptr<T, void> & _right )
    {
        return !(_left == _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator != ( const intrusive_ptr<T, void> & _left, const U * _right )
    {
        const T * ptr = _left.get();

        return ptr != static_cast<const T *>(_right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator != ( const U * _left, const intrusive_ptr<T, void> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != static_cast<const T *>(_left);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator == ( const U * _left, const intrusive_ptr<T, void> & _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T, class U>
    bool operator == ( const intrusive_ptr<T, void> & _left, const U * _right )
    {
        return !(_left != _right);
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( const intrusive_ptr<T, void> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator != ( std::nullptr_t, const intrusive_ptr<T, void> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr != nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( std::nullptr_t, const intrusive_ptr<T, void> & _right )
    {
        const T * ptr = intrusive_get<const T *>( _right );

        return ptr == nullptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    bool operator == ( const intrusive_ptr<T, void> & _left, std::nullptr_t )
    {
        const T * ptr = _left.get();

        return ptr == nullptr;
    }
}