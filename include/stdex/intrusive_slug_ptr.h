#pragma once

#include "stdex/intrusive_slug_linked_ptr.h"
#include "stdex/intrusive_slug_list_size_ptr.h"
#include "stdex/intrusive_slug_linked_ptr.h"

namespace stdex
{
    template<class C>
    class intrusive_slug_ptr
        : public intrusive_slug_linked_ptr<typename C::value_type>
    {
    public:
        typedef typename C::value_type value_type;
        typedef intrusive_ptr<value_type> value_type_ptr;
        typedef C list_type;
        typedef intrusive_slug_linked_ptr<value_type> linked_type;
        typedef intrusive_ptr<linked_type> linked_type_ptr;

    public:
        explicit intrusive_slug_ptr( const list_type & _list )
            : intrusive_slug_linked_ptr<value_type>( EILT_SLUG )
            , m_list( _list )
        {
            intrusive_this_acquire( this );

            m_list.increfSlug( linked_type_ptr( this ) );
        }

        ~intrusive_slug_ptr()
        {
            m_list.decrefSlug();
        }

    public:
        void next_shuffle()
        {
            const linked_type * pos_right = this->right_get();

            const linked_type * head = m_list.head();

            const linked_type * pos_right_adapt_right_adapt = nullptr;

            uint32_t countSlugs = m_list.countSlugs();

            if( countSlugs == 1 )
            {
                if( pos_right == head )
                {
                    return;
                }

                pos_right_adapt_right_adapt = pos_right->right_get();
            }
            else
            {
                const linked_type * pos_right_adapt = this->adapt_( pos_right );

                if( pos_right_adapt == head )
                {
                    return;
                }

                const linked_type * pos_right_adapt_right = pos_right_adapt->right_get();

                pos_right_adapt_right_adapt = this->adapt_( pos_right_adapt_right );
            }

            this->unlink();

            pos_right_adapt_right_adapt->link_before( linked_type_ptr( this ) );
        }

        inline bool eof() const
        {
            const linked_type * pos = this->adapt_right();

            const linked_type * head = m_list.head();

            return pos == head;
        }

        inline const value_type * operator -> () const
        {
            const linked_type * linked = this->current();

            return static_cast<const value_type *>(linked);
        }

        inline const value_type * operator * () const
        {
            const linked_type * linked = this->current();

            return static_cast<const value_type *>(linked);
        }

        inline value_type * operator -> ()
        {
            linked_type * linked = this->current();

            return static_cast<value_type *>(linked);
        }

        inline value_type * operator * ()
        {
            linked_type * linked = this->current();

            return static_cast<value_type *>(linked);
        }

    protected:
        inline linked_type * current()
        {
            linked_type * pos = this->adapt_right();

            return pos;
        }

        inline const linked_type * current() const
        {
            const linked_type * pos = this->adapt_right();

            return pos;
        }

        inline const linked_type * adapt_( const linked_type * _pos ) const
        {
            const linked_type * pos = _pos;

            while( pos->getIntrusiveTag() == EILT_SLUG )
            {
                pos = pos->right_get();
            }

            return pos;
        }

        inline linked_type * adapt_right()
        {
            linked_type * pos = this->right_get();

            while( pos->getIntrusiveTag() == EILT_SLUG )
            {
                pos = pos->right_get();
            }

            return pos;
        }

        inline const linked_type * adapt_right() const
        {
            const linked_type * pos = this->right_get();

            while( pos->getIntrusiveTag() == EILT_SLUG )
            {
                pos = pos->right_get();
            }

            return pos;
        }

    protected:
        void destroy() override
        {
            //Empty
        }

    protected:
        const list_type & m_list;
    };
}