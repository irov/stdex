#pragma once

#include "stdex/intrusive_ptr_config.h"

#include <utility>

namespace stdex
{
    template<class T, class D>
    class intrusive_ptr_view;

    namespace detail
    {
        struct intrusive_adopt_t
        {
        };
    }

    template<class T, class D = void>
    class intrusive_ptr
        : public intrusive_ptr<D, void>
    {
        template<class U, class UD>
        friend class intrusive_ptr;

        template<class U, class UD>
        friend class intrusive_ptr_view;

    public:
        typedef T value_type;
        typedef D derived_type;
        typedef value_type * pointer_type;
        typedef const value_type * const_pointer_type;
        typedef intrusive_ptr<derived_type, void> derived_type_ptr;

    public:
        static inline const intrusive_ptr & none()
        {
            static const intrusive_ptr ptr_none;

            return ptr_none;
        }

    public:
        constexpr intrusive_ptr()
            : intrusive_ptr<derived_type>( nullptr )
        {
        }

        constexpr intrusive_ptr( std::nullptr_t )
            : intrusive_ptr<derived_type>( nullptr )
        {
        }

        constexpr intrusive_ptr( const intrusive_ptr & _rhs )
            : intrusive_ptr<derived_type>( _rhs.m_ptr )
        {
        }

        constexpr intrusive_ptr( intrusive_ptr && _rhs ) noexcept
            : intrusive_ptr<derived_type>( std::forward<intrusive_ptr>( _rhs ) )
        {
        }

        constexpr intrusive_ptr( const derived_type_ptr & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        constexpr intrusive_ptr( derived_type_ptr && _rhs ) noexcept
            : derived_type_ptr( intrusive_ptr::move_from( _rhs ), detail::intrusive_adopt_t() )
        {
        }

        constexpr explicit intrusive_ptr( const value_type * _ptr )
            : derived_type_ptr( _ptr )
        {
        }

    private:
        constexpr explicit intrusive_ptr( const value_type * _ptr, detail::intrusive_adopt_t ) noexcept
            : derived_type_ptr( _ptr, detail::intrusive_adopt_t() )
        {
        }

    public:
        template<class U>
        constexpr intrusive_ptr( const intrusive_ptr<U> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        template<class U>
        constexpr intrusive_ptr( intrusive_ptr<U> && _rhs ) noexcept
            : derived_type_ptr( intrusive_ptr::move_from( _rhs ), detail::intrusive_adopt_t() )
        {
        }

        template<class U, class UD>
        constexpr intrusive_ptr( const intrusive_ptr<U, UD> & _rhs )
            : intrusive_ptr<derived_type>( _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
        }

        template<class U, class UD>
        constexpr intrusive_ptr( intrusive_ptr<U, UD> && _rhs ) noexcept
            : derived_type_ptr( intrusive_ptr::move_from( _rhs ), detail::intrusive_adopt_t() )
        {
        }

        template<class U>
        constexpr explicit intrusive_ptr( const U * _ptr )
            : intrusive_ptr<derived_type>( _ptr )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, pointer_type );
        }

        ~intrusive_ptr()
        {
        }

    private:
        intrusive_ptr( bool ) = delete;

        template<class U, class UD>
        static derived_type * move_from( intrusive_ptr<U, UD> & _rhs ) noexcept
        {
            U * ptr = _rhs.get();

            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( ptr, pointer_type );

            _rhs.detach();

            return static_cast<derived_type *>(ptr);
        }

    public:
        static intrusive_ptr from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U>
        static intrusive_ptr from( U _pointer )
        {
            return intrusive_ptr( static_cast<const value_type *>(_pointer) );
        }

        template<class U, class UD>
        static intrusive_ptr from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.template getT<pointer_type>() );
        }

        static intrusive_ptr adopt( pointer_type _pointer ) noexcept
        {
            return intrusive_ptr( _pointer, detail::intrusive_adopt_t() );
        }

    public:
        static intrusive_ptr dynamic_from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U>
        static intrusive_ptr dynamic_from( U _pointer )
        {
            return intrusive_ptr( dynamic_cast<const value_type *>(_pointer) );
        }

        template<class U, class UD>
        static intrusive_ptr dynamic_from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.template getDynamicT<pointer_type>() );
        }

    public:
        template<class U, class UD>
        static pointer_type ptr( const intrusive_ptr<U, UD> & _pointer )
        {
            return _pointer.template getT<pointer_type>();
        }

    public:
        constexpr intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( intrusive_ptr<derived_type>::m_ptr == _rhs.m_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        constexpr intrusive_ptr & operator = ( intrusive_ptr && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( std::forward<intrusive_ptr>(_rhs) );
            swap_ptr.swap( *this );

            return *this;
        }

        constexpr intrusive_ptr & operator = ( value_type * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( intrusive_ptr<derived_type>::m_ptr == static_cast<derived_type *>(_rhs) )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        constexpr intrusive_ptr & operator = ( const intrusive_ptr<U, UD> & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            const derived_type * rhs_ptr = static_cast<const derived_type *>(_rhs.get());

            if( intrusive_ptr<derived_type>::m_ptr == rhs_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U, class UD>
        constexpr intrusive_ptr & operator = ( intrusive_ptr<U, UD> && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( std::forward<intrusive_ptr<U, UD>>(_rhs) );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        constexpr intrusive_ptr & operator = ( U * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            const derived_type * rhs_ptr = static_cast<const derived_type *>(_rhs);

            if( intrusive_ptr<derived_type>::m_ptr == rhs_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        constexpr intrusive_ptr & operator = ( std::nullptr_t )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( intrusive_ptr<derived_type>::m_ptr == nullptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        constexpr value_type * get() const
        {
            value_type * ptr = intrusive_ptr<derived_type>::template getT<value_type *>();

            return ptr;
        }

        template<class K>
        constexpr K getT() const
        {
            value_type * ptr = this->get();

            K ptr_t = static_cast<K>(ptr);

            return ptr_t;
        }

        template<class K>
        constexpr K getDynamicT() const
        {
            value_type * ptr = this->get();

            K ptr_t = dynamic_cast<K>(ptr);

            return ptr_t;
        }

        constexpr value_type * detach() noexcept
        {
            derived_type * ptr = intrusive_ptr<derived_type>::detach();

            return static_cast<value_type *>(ptr);
        }

        template<class U, class UD>
        constexpr intrusive_ptr<U, UD> moveT() noexcept
        {
            value_type * ptr = this->detach();

            U * ptr_t = static_cast<U *>(ptr);

            return intrusive_ptr<U, UD>( ptr_t, detail::intrusive_adopt_t() );
        }

        constexpr value_type * operator -> () const
        {
            derived_type * ptr = intrusive_ptr<derived_type>::operator ->();

            return static_cast<value_type *>(ptr);
        }

    public:
        constexpr value_type * acquire() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            T::intrusive_ptr_add_ref( intrusive_ptr<derived_type>::m_ptr );

            return static_cast<value_type *>(intrusive_ptr<derived_type>::m_ptr);
        }

        static void release( value_type * _ptr )
        {
            T::intrusive_ptr_dec_ref( _ptr );
        }

    public:
        constexpr void swap( intrusive_ptr & _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            derived_type * tmp = intrusive_ptr<derived_type>::m_ptr;
            intrusive_ptr<derived_type>::m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T, class D>
    void swap( intrusive_ptr<T, D> & _left, intrusive_ptr<T, D> & _right ) noexcept
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

        STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( t_ptr, U );

        return u_ptr;
    }
    //////////////////////////////////////////////////////////////////////////
    template<class U, class T, class D>
    U intrusive_dynamic_get( const intrusive_ptr<T, D> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = dynamic_cast<U>(t_ptr);

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
        template<class U, class UD>
        friend class intrusive_ptr;

        template<class U, class UD>
        friend class intrusive_ptr_view;

    public:
        typedef T value_type;
        typedef void derived_type;
        typedef T * pointer_type;
        typedef const T * const_pointer_type;

    public:
        static inline const intrusive_ptr & none()
        {
            static const intrusive_ptr ptr_none;

            return ptr_none;
        }

    public:
        constexpr intrusive_ptr()
            : m_ptr( nullptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        constexpr intrusive_ptr( std::nullptr_t )
            : m_ptr( nullptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        constexpr intrusive_ptr( const intrusive_ptr & _rhs )
            : m_ptr( _rhs.get() )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        constexpr intrusive_ptr( intrusive_ptr && _rhs ) noexcept
            : m_ptr( _rhs.m_ptr )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            _rhs.m_ptr = nullptr;
        }

        constexpr explicit intrusive_ptr( const value_type * _ptr )
            : m_ptr( const_cast<pointer_type>(_ptr) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, const_pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

    private:
        constexpr explicit intrusive_ptr( const value_type * _ptr, detail::intrusive_adopt_t ) noexcept
            : m_ptr( const_cast<pointer_type>(_ptr) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _ptr, const_pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

    public:
        template<class U>
        constexpr intrusive_ptr( const intrusive_ptr<U, void> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U>
        constexpr intrusive_ptr( intrusive_ptr<U, void> && _rhs ) noexcept
            : m_ptr( intrusive_ptr::move_from( _rhs ) )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        template<class U, class Y>
        constexpr intrusive_ptr( const intrusive_ptr<U, Y> & _rhs )
            : m_ptr( static_cast<T *>(_rhs.get()) )
        {
            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( _rhs.get(), pointer_type );
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();

            this->incref();
        }

        template<class U, class Y>
        constexpr intrusive_ptr( intrusive_ptr<U, Y> && _rhs ) noexcept
            : m_ptr( intrusive_ptr::move_from( _rhs ) )
        {
            STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK();
        }

        template<class U>
        constexpr explicit intrusive_ptr( const U * _ptr )
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

    private:
        intrusive_ptr( bool ) = delete;

        template<class U, class Y>
        static pointer_type move_from( intrusive_ptr<U, Y> & _rhs ) noexcept
        {
            U * ptr = _rhs.get();

            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( ptr, pointer_type );

            _rhs.detach();

            return static_cast<pointer_type>(ptr);
        }

    public:
        static intrusive_ptr from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U>
        static intrusive_ptr from( U _pointer )
        {
            return intrusive_ptr( static_cast<const value_type *>(_pointer) );
        }

        template<class U, class UD>
        static intrusive_ptr from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.get() );
        }

        static intrusive_ptr adopt( pointer_type _pointer ) noexcept
        {
            return intrusive_ptr( _pointer, detail::intrusive_adopt_t() );
        }

    public:
        static intrusive_ptr dynamic_from( const_pointer_type _pointer )
        {
            return intrusive_ptr( _pointer );
        }

        template<class U>
        static intrusive_ptr dynamic_from( U _pointer )
        {
            return intrusive_ptr( dynamic_cast<const value_type *>(_pointer) );
        }

        template<class U, class UD>
        static intrusive_ptr dynamic_from( const intrusive_ptr<U, UD> & _pointer )
        {
            return intrusive_ptr( _pointer.template getDynamicT<pointer_type>() );
        }

    public:
        template<class U, class UD>
        static pointer_type ptr( const intrusive_ptr<U, UD> & _pointer )
        {
            return _pointer.template getT<pointer_type>();
        }

    public:
        constexpr intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr == _rhs.m_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        intrusive_ptr & operator = ( intrusive_ptr && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( this == &_rhs )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( std::forward<intrusive_ptr>(_rhs) );
            swap_ptr.swap( *this );

            return *this;
        }

        constexpr intrusive_ptr & operator = ( value_type * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr == _rhs )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        constexpr intrusive_ptr & operator = ( const intrusive_ptr<U, void> & _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            const_pointer_type rhs_ptr = static_cast<const_pointer_type>(_rhs.get());

            if( m_ptr == rhs_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        constexpr intrusive_ptr & operator = ( intrusive_ptr<U, void> && _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr( std::forward<intrusive_ptr<U, void>>(_rhs) );
            swap_ptr.swap( *this );

            return *this;
        }

        template<class U>
        constexpr intrusive_ptr & operator = ( U * _rhs )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            const_pointer_type rhs_ptr = static_cast<const_pointer_type>(_rhs);

            if( m_ptr == rhs_ptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr( _rhs );
            swap_ptr.swap( *this );

            return *this;
        }

        constexpr intrusive_ptr & operator = ( std::nullptr_t )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr == nullptr )
            {
                return *this;
            }

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );

            return *this;
        }

    public:
        constexpr void set( value_type * _ptr )
        {
            this->set_unchecked( _ptr );
        }

        constexpr void set_unchecked( value_type * _ptr )
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            m_ptr = _ptr;
        }

        constexpr value_type * get_base() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            return m_ptr;
        }

        constexpr value_type * get() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            return m_ptr;
        }

        template<class K>
        constexpr K getT() const
        {
            value_type * ptr = this->get();

            STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR( ptr, K );

            K ptr_t = static_cast<K>(ptr);

            return ptr_t;
        }

        template<class K>
        constexpr K getDynamicT() const
        {
            value_type * ptr = this->get();

            K ptr_t = dynamic_cast<K>(ptr);

            return ptr_t;
        }

        constexpr value_type * detach() noexcept
        {
            value_type * ptr = m_ptr;
            m_ptr = nullptr;

            return ptr;
        }

        template<class U, class D>
        constexpr intrusive_ptr<U, D> moveT() noexcept
        {
            value_type * ptr = this->detach();

            U * ptr_t = static_cast<U *>(ptr);

            return intrusive_ptr<U, D>( ptr_t, detail::intrusive_adopt_t() );
        }


        constexpr value_type * operator -> () const
        {
#   ifdef STDEX_INTRUSIVE_PTR_DEBUG
            if( m_ptr == nullptr )
            {
                STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR;
            }

            if( T::intrusive_ptr_check_ref( m_ptr ) == false )
            {
                STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR;
            }
#   endif

            return m_ptr;
        }

    public:
        constexpr value_type * acquire() const
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();
            
            T::intrusive_ptr_add_ref( m_ptr );

            return m_ptr;
        }

        static void release( value_type * _ptr )
        {
            T::intrusive_ptr_dec_ref( _ptr );
        }

    public:
        void reset() noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            intrusive_ptr swap_ptr;
            swap_ptr.swap( *this );
        }

        constexpr void swap( intrusive_ptr & _rhs ) noexcept
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            T * tmp = m_ptr;
            m_ptr = _rhs.m_ptr;
            _rhs.m_ptr = tmp;
        }

    protected:
        constexpr void incref()
        {
            STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK();

            if( m_ptr != nullptr )
            {
                T::intrusive_ptr_add_ref( m_ptr );
            }
        }

        constexpr void decref()
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
    void swap( intrusive_ptr<T, void> & _left, intrusive_ptr<T, void> & _right ) noexcept
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
    template<class U, class T>
    U intrusive_dynamic_get( const intrusive_ptr<T, void> & _iptr )
    {
        T * t_ptr = _iptr.get();
        U u_ptr = dynamic_cast<U>(t_ptr);

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
