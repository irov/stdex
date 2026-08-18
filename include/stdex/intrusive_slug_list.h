#pragma once

#include "stdex/intrusive_slug_linked.h"
#include "stdex/intrusive_slug_algorithm.h"
#include "stdex/intrusive_algorithm.h"

namespace stdex
{
    template<class T>
    class intrusive_slug_list
    {
    public:
        typedef uint32_t size_type;
        typedef intrusive_slug_linked<T> linked_type;
        typedef T * value_type;

    public:
        intrusive_slug_list()
            : m_head( EILT_END )
        {
            m_head.m_right = &m_head;
            m_head.m_left = &m_head;
        }

        ~intrusive_slug_list()
        {
            this->clear();

            m_head.m_right = nullptr;
            m_head.m_left = nullptr;
        }

    private:
        intrusive_slug_list( const intrusive_slug_list & ) = delete;
        intrusive_slug_list & operator = ( const intrusive_slug_list & ) = delete;

    protected:
        template<class It>
        class base_iterator
        {
        public:
            inline explicit base_iterator( linked_type * _node )
                : m_node( _node )
            {
                this->adapt_node();
            }

            inline base_iterator( linked_type * _node, detail::intrusive_slug_no_adapt_t )
                : m_node( _node )
            {
            }

            inline base_iterator( const base_iterator & _node )
                : m_node( _node.get() )
            {
            }

        public:
            inline T * operator -> () const
            {
                return static_cast<T *>(m_node);
            }

            inline T * operator * () const
            {
                return static_cast<T *>(m_node);
            }

            inline linked_type * get() const
            {
                return m_node;
            }

        public:
            inline bool operator == ( It _it ) const
            {
                return m_node == _it.m_node;
            }

            inline bool operator != ( It _it ) const
            {
                return !this->operator == ( _it );
            }

        protected:
            inline void shuffle_next()
            {
                do
                {
                    m_node = m_node->right();
                } while( m_node->getIntrusiveTag() == EILT_SLUG );
            }

            inline void shuffle_prev()
            {
                do
                {
                    m_node = m_node->left();
                } while( m_node->getIntrusiveTag() == EILT_SLUG );
            }

            inline void adapt_node()
            {
                while( m_node->getIntrusiveTag() == EILT_SLUG )
                {
                    m_node = m_node->right();
                }
            }

            inline void adapt_node_reverse()
            {
                while( m_node->getIntrusiveTag() == EILT_SLUG )
                {
                    m_node = m_node->left();
                }
            }

        protected:
            linked_type * m_node;
        };

    public:
        class const_iterator
            : public base_iterator<const_iterator>
        {
        public:
            inline explicit const_iterator( linked_type * _node )
                : base_iterator<const_iterator>( _node )
            {
            }

            inline const_iterator( linked_type * _node, detail::intrusive_slug_no_adapt_t )
                : base_iterator<const_iterator>( _node, detail::intrusive_slug_no_adapt_t() )
            {
            }

            inline const_iterator( const const_iterator & _node )
                : base_iterator<const_iterator>( _node )
            {
            }

        public:
            inline const_iterator & operator = ( const const_iterator & _it )
            {
                this->m_node = _it.m_node;

                this->adapt_node();

                return *this;
            }

        public:
            inline const_iterator & operator ++ ()
            {
                this->shuffle_next();

                return *this;
            }

            inline const_iterator operator ++ ( int )
            {
                const_iterator tmp = *this;
                ++ * this;
                return tmp;
            }

            inline const_iterator & operator -- ()
            {
                this->shuffle_prev();

                return *this;
            }

            inline const_iterator operator -- ( int )
            {
                const_iterator tmp = *this;
                -- * this;
                return tmp;
            }
        };

        class iterator
            : public base_iterator<iterator>
        {
        public:
            inline explicit iterator( linked_type * _node )
                : base_iterator<iterator>( _node )
            {
            }

            inline iterator( linked_type * _node, detail::intrusive_slug_no_adapt_t )
                : base_iterator<iterator>( _node, detail::intrusive_slug_no_adapt_t() )
            {
            }

            inline iterator( const iterator & _node )
                : base_iterator<iterator>( _node )
            {
            }

        public:
            inline iterator & operator = ( const iterator & _it )
            {
                this->m_node = _it.m_node;

                this->adapt_node();

                return *this;
            }

            inline iterator & operator ++ ()
            {
                this->shuffle_next();

                return *this;
            }

            inline iterator operator ++ ( int )
            {
                iterator tmp = *this;
                ++ * this;
                return tmp;
            }

            inline iterator & operator -- ()
            {
                this->shuffle_prev();

                return *this;
            }

            inline iterator operator -- ( int )
            {
                iterator tmp = *this;
                -- * this;
                return tmp;
            }
        };

        class reverse_iterator
            : public base_iterator<reverse_iterator>
        {
        public:
            inline explicit reverse_iterator( linked_type * _node )
                : base_iterator<reverse_iterator>( _node, detail::intrusive_slug_no_adapt_t() )
            {
                this->adapt_node_reverse();
            }

            inline reverse_iterator( linked_type * _node, detail::intrusive_slug_no_adapt_t )
                : base_iterator<reverse_iterator>( _node, detail::intrusive_slug_no_adapt_t() )
            {
            }

            inline reverse_iterator( const reverse_iterator & _node )
                : base_iterator<reverse_iterator>( _node )
            {
            }

        public:
            inline reverse_iterator & operator = ( const reverse_iterator & _it )
            {
                this->m_node = _it.m_node;

                return *this;
            }

        public:
            inline reverse_iterator & operator ++ ()
            {
                this->shuffle_prev();

                return *this;
            }

            inline reverse_iterator operator ++ ( int )
            {
                reverse_iterator tmp = *this;
                ++ * this;
                return tmp;
            }

            inline reverse_iterator & operator -- ()
            {
                this->shuffle_next();

                return *this;
            }

            inline reverse_iterator operator -- ( int )
            {
                reverse_iterator tmp = *this;
                -- * this;
                return tmp;
            }
        };

    public:
        inline iterator begin()
        {
            return iterator( m_head.m_right );
        }

        inline iterator end()
        {
            return iterator( &m_head, detail::intrusive_slug_no_adapt_t() );
        }

        inline const_iterator begin() const
        {
            return const_iterator( m_head.m_right );
        }

        inline const_iterator end() const
        {
            return const_iterator( &m_head, detail::intrusive_slug_no_adapt_t() );
        }

        inline reverse_iterator rbegin()
        {
            return reverse_iterator( m_head.m_left );
        }

        inline reverse_iterator rend()
        {
            return reverse_iterator( &m_head, detail::intrusive_slug_no_adapt_t() );
        }

    public:
        inline T * front() const
        {
            return *this->begin();
        }

        inline T * back() const
        {
            return *(--this->end());
        }

    public:
        inline void push_front( linked_type * _node )
        {
            iterator it = this->pure_begin_();

            this->insert_( it, _node );
        }

        inline void pop_front()
        {
            iterator it = this->begin();

            this->erase( it );
        }

        inline void push_back( linked_type * _node )
        {
            iterator it = this->end();
            this->insert_( it, _node );
        }

        inline void pop_back()
        {
            iterator it = this->end();
            this->erase( --it );
        }

        inline void splice( iterator _from, iterator _where )
        {
            if( _from == this->end() || _from == _where )
            {
                return;
            }

            iterator it_next = _from;
            ++it_next;

            if( it_next == _where )
            {
                return;
            }

            linked_type * node = _from.get();
            linked_type * where = _where.get();

            node->unlink();
            where->link_before( node );
        }

        inline void clear()
        {
            iterator it = this->begin();
            iterator it_end = this->end();
            while( it != it_end )
            {
                it = this->erase( it );
            }
        }

        inline size_type size() const
        {
            const_iterator it = this->begin();
            const_iterator it_end = this->end();
            size_type count = stdex::helper::intrusive_slug_distance( it, it_end );

            return count;
        }

        inline bool empty() const
        {
            const_iterator it = this->begin();
            const_iterator it_end = this->end();
            bool empty = stdex::helper::intrusive_slug_empty( it, it_end );

            return empty;
        }

        inline iterator insert( iterator _where, linked_type * _node )
        {
            this->insert_( _where, _node );

            return (--_where);
        }

        inline void insert( iterator _where, iterator _begin, iterator _end )
        {
            if( _begin == _end || _where == _begin || _where == _end )
            {
                return;
            }

            linked_type * first = _begin.get();
            linked_type * after = _end.get();
            linked_type * where = _where.get();

            for( linked_type * it = first; it != after; it = it->right() )
            {
                if( it == where || it->getIntrusiveTag() == EILT_SLUG )
                {
                    return;
                }
            }

            linked_type * last = after->m_left;
            linked_type * before = first->m_left;

            linked_type * where_before = where->m_left;

            before->m_right = after;
            after->m_left = before;

            where_before->m_right = first;
            first->m_left = where_before;

            last->m_right = where;
            where->m_left = last;
        }

        inline iterator erase( iterator _where )
        {
            iterator it = _where++;

            if( it != this->end() )
            {
                linked_type * node = it.get();
                node->unlink();
            }

            return (_where);
        }

        template<class F>
        inline iterator erase_if( F _pred )
        {
            iterator it_found = stdex::helper::intrusive_find_if( this->begin(), this->end(), _pred );

            if( it_found == this->end() )
            {
                return this->end();
            }

            return this->erase( it_found );
        }

    protected:
        inline void insert_( iterator _where, linked_type * _node )
        {
            linked_type * linked = _where.get();
            linked->link_before( _node );
        }

        inline iterator pure_begin_()
        {
            return iterator( m_head.m_right, detail::intrusive_slug_no_adapt_t() );
        }

    protected:
        mutable linked_type m_head;
    };
}
