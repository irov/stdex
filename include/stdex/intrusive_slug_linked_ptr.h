#pragma once

#include "stdex/intrusive_slug_linked_tag.h"
#include "stdex/intrusive_ptr_base.h"

namespace stdex
{
    template<class Tag>
    class intrusive_slug_linked_ptr
        : virtual public stdex::intrusive_ptr_base
    {
    public:
        typedef intrusive_slug_linked_ptr<Tag> linked_type;
        typedef intrusive_ptr<linked_type> linked_type_ptr;

    public:
        inline intrusive_slug_linked_ptr()
            : m_tag( EILT_ELEMENT )
        {
        }

        inline intrusive_slug_linked_ptr( EIntrusiveSlugLinkedTag _tag )
            : m_tag( _tag )
        {
        }

        inline ~intrusive_slug_linked_ptr()
        {
            this->unlink();
        }

    private:
        inline intrusive_slug_linked_ptr( const intrusive_slug_linked_ptr & )
        {
        }

        inline intrusive_slug_linked_ptr & operator = ( const intrusive_slug_linked_ptr & )
        {
            return *this;
        }

    public:
        inline int getIntrusiveTag() const
        {
            return m_tag;
        }

        inline bool unique() const
        {
            return (m_right == nullptr) && (m_left == nullptr);
        }

        inline const linked_type_ptr & left() const
        {
            return m_left;
        }

        inline const linked_type_ptr & right() const
        {
            return m_right;
        }

        inline const linked_type * left_get() const
        {
            return m_left.get();
        }

        inline const linked_type * right_get() const
        {
            return m_right.get();
        }

        inline linked_type * left_get()
        {
            return m_left.get();
        }

        inline linked_type * right_get()
        {
            return m_right.get();
        }

        inline void link_after( const linked_type_ptr & _other ) const
        {
            _other->m_right = m_right;
            _other->m_left = linked_type_ptr( this );

            if( m_right != nullptr )
            {
                m_right->m_left = _other;
            }

            m_right = _other;
        }

        inline void link_before( const linked_type_ptr & _other ) const
        {
            _other->m_left = m_left;
            _other->m_right = linked_type_ptr( this );

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

        inline linked_type_ptr leftcast() const
        {
            linked_type_ptr it = m_left;

            while( it->m_left != nullptr )
            {
                it = it->m_left;
            }

            return it;
        }

        inline linked_type_ptr rightcast() const
        {
            linked_type_ptr it = m_right;

            while( it->m_right != nullptr )
            {
                it = it->m_right;
            }

            return it;
        }

        void linkall( const linked_type_ptr & _other )
        {
            const linked_type_ptr & other_right = _other->m_right;

            if( m_left != nullptr )
            {
                linked_type_ptr left = this->leftcast();

                left->m_left = _other;
                _other->m_right = left;
            }
            else
            {
                m_left = _other;
                _other->m_right = linked_type_ptr( this );
            }

            if( other_right != nullptr )
            {
                if( m_right != nullptr )
                {
                    linked_type_ptr right = this->rightcast();

                    other_right->m_left = right;
                    right->m_right = other_right;
                }
                else
                {
                    other_right->m_left = linked_type_ptr( this );
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
            linked_type_ptr nc_this( this );
            _f( nc_this );
        }

        template<class F>
        void foreach_other( F _pred ) const
        {
            linked_type_ptr it_right = m_right;
            while( it_right != nullptr )
            {
                _pred( it_right );
                it_right = it_right->m_right;
            }

            linked_type_ptr it_left = m_left;
            while( it_left != nullptr )
            {
                _pred( (it_left) );
                it_left = it_left->m_left;
            }
        }

        template<class F>
        linked_type_ptr find( F _pred ) const
        {
            linked_type_ptr node_found = this->find_self( _pred );

            if( node_found != nullptr )
            {
                return node_found;
            }

            linked_type_ptr other_node = this->find_other( _pred );

            return other_node;
        }

        template<class F>
        linked_type_ptr find_self( F _pred ) const
        {
            if( _pred( this ) == true )
            {
                return this;
            }

            return nullptr;
        }

        template<class F>
        linked_type_ptr find_other( F _pred ) const
        {
            linked_type_ptr it_right = m_right;

            while( it_right != nullptr )
            {
                if( _pred( it_right ) == true )
                {
                    return this;
                }

                it_right = it_right->m_right;
            }

            linked_type_ptr it_left = m_left;

            while( it_left != nullptr )
            {
                if( _pred( it_left ) == true )
                {
                    return this;
                }

                it_left = it_left->m_left;
            }

            return nullptr;
        }

    public:
        mutable linked_type_ptr m_right;
        mutable linked_type_ptr m_left;
        int m_tag;
    };
}