#pragma once

#include "stdex/intrusive_ptr_base.h"
#include "stdex/intrusive_ptr.h"

namespace stdex
{
    template<class T, class D = void>
    class intrusive_linked_ptr
        : public intrusive_ptr_base
    {
    public:
        typedef T value_type;
        typedef D derived_type;
        typedef intrusive_ptr<T, D> value_type_ptr;

    public:
        inline intrusive_linked_ptr()
        {
        }

        inline ~intrusive_linked_ptr()
        {
            this->unlink();
        }

    private:
        intrusive_linked_ptr & operator = ( const intrusive_linked_ptr & _linked )
        {
            (void)_linked;

            return *this;
        }

    public:
        inline bool unique() const
        {
            return (m_right == nullptr) && (m_left == nullptr);
        }

        inline const value_type_ptr & left() const
        {
            return m_left;
        }

        inline const value_type_ptr & right() const
        {
            return m_right;
        }

        inline void link_after( const value_type_ptr & _other )
        {
            _other->m_right = m_right;
            _other->m_left = value_type_ptr( static_cast<T *>(this) );

            if( m_right != nullptr )
            {
                m_right->m_left = _other;
            }

            m_right = _other;
        }

        inline void link_before( const value_type_ptr & _other )
        {
            _other->m_left = m_left;
            _other->m_right = value_type_ptr( static_cast<T *>(this) );

            if( m_left != nullptr )
            {
                m_left->m_right = _other;
            }

            m_left = _other;
        }

        inline void unlink()
        {
            if( m_right != nullptr )
            {
                m_right->m_left = m_left;
            }

            if( m_left != nullptr )
            {
                m_left->m_right = m_right;
            }

            m_left = nullptr;
            m_right = nullptr;
        }

        inline value_type_ptr leftcast() const
        {
            value_type_ptr it = m_left;

            while( it->m_left != nullptr )
            {
                it = it->m_left;
            }

            return it;
        }

        inline value_type_ptr rightcast() const
        {
            value_type_ptr it = m_right;

            while( it->m_right != nullptr )
            {
                it = it->m_right;
            }

            return it;
        }

        void linkall( const value_type_ptr & _other )
        {
            const value_type_ptr & other_right = _other->m_right;

            if( m_left != nullptr )
            {
                value_type_ptr left = this->leftcast();

                left->m_left = _other;
                _other->m_right = left;
            }
            else
            {
                m_left = _other;
                _other->m_right = value_type_ptr( static_cast<T *>(this) );
            }

            if( m_right != nullptr )
            {
                if( other_right != nullptr )
                {
                    value_type_ptr right = this->rightcast();

                    other_right->m_left = right;
                    right->m_right = other_right;
                }
            }
            else
            {
                if( other_right != nullptr )
                {
                    other_right->m_left = value_type_ptr( static_cast<T *>(this) );
                    m_right = other_right;
                }
            }
        }

        template<class F>
        void foreach( F _f ) const
        {
            this->foreach_self<F>( _f );
            this->foreach_other<F>( _f );
        }

        template<class F>
        void foreach_self( F _f ) const
        {
            const value_type * nc_this = static_cast<const value_type *>(this);
            (_f)(const_cast<value_type *>(nc_this));
        }

        template<class F>
        void foreach_other( F _pred ) const
        {
            linked_type * it_right = m_right;
            while( it_right != nullptr )
            {
                _pred( it_right );
                it_right = it_right->m_right;
            }

            linked_type * it_left = m_left;
            while( it_left != nullptr )
            {
                _pred( (it_left) );
                it_left = it_left->m_left;
            }
        }

        template<class F>
        const value_type_ptr find( F _pred ) const
        {
            value_type_ptr node_found = this->find_self( _pred );

            if( node_found != nullptr )
            {
                return node_found;
            }

            value_type_ptr other = this->find_other( _pred );

            return other;
        }

        template<class F>
        value_type_ptr find_self( F _pred ) const
        {
            if( _pred( this ) == true )
            {
                return static_cast<value_type *>(this);
            }

            return nullptr;
        }

        template<class F>
        value_type_ptr find_other( F _pred ) const
        {
            value_type_ptr it_right = m_right;

            while( it_right != nullptr )
            {
                if( _pred( it_right ) == true )
                {
                    return static_cast<value_type *>(this);
                }

                it_right = it_right->m_right;
            }

            value_type_ptr it_left = m_left;

            while( it_left )
            {
                if( _pred( it_left ) == true )
                {
                    return static_cast<value_type *>(this);
                }

                it_left = it_left->m_left;
            }

            return nullptr;
        }

    public:
        mutable value_type_ptr m_right;
        mutable value_type_ptr m_left;
    };
}