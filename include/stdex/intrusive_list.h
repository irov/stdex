#pragma once

#include "stdex/intrusive_linked.h"
#include "stdex/intrusive_algorithm.h"

namespace stdex
{
    template<class T>
    class intrusive_list
    {
    public:
        typedef uint32_t size_type;
        typedef typename stdex::intrusive_linked<T>::linked_type linked_type;
        typedef T * value_type;

    public:
        intrusive_list()
        {
            m_head.m_right = &m_head;
            m_head.m_left = &m_head;
        }

        ~intrusive_list()
        {
            this->clear();

            m_head.m_right = nullptr;
            m_head.m_left = nullptr;
        }

    protected:
        template<class It>
        class base_iterator
        {
        public:
            inline explicit base_iterator( linked_type * _node )
                : m_node( _node )
            {
            }

            inline base_iterator( const base_iterator & _it )
                : m_node( _it.get() )
            {
            }

        public:
            base_iterator & operator = ( const base_iterator & _it )
            {
                this->m_node = _it.get();

                return *this;
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
                m_node = m_node->right();
            }

            inline void shuffle_prev()
            {
                m_node = m_node->left();
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

            inline const_iterator( const const_iterator & _node )
                : base_iterator<const_iterator>( _node )
            {
            }

        public:
            inline const_iterator & operator = ( const const_iterator & _it )
            {
                this->m_node = _it.m_node;

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
                ++*this;

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
                --*this;

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

            inline iterator( const iterator & _node )
                : base_iterator<iterator>( _node )
            {
            }

        public:
            inline iterator & operator = ( const iterator & _it )
            {
                this->m_node = _it.m_node;

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
                ++*this;

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
                --*this;

                return tmp;
            }
        };

        class reverse_iterator
            : public base_iterator<reverse_iterator>
        {
        public:
            inline explicit reverse_iterator( linked_type * _node )
                : base_iterator<reverse_iterator>( _node )
            {
            }

            inline reverse_iterator( const reverse_iterator & _node )
                : base_iterator<reverse_iterator>( _node )
            {
            }

        private:
            inline reverse_iterator & operator = ( const reverse_iterator & )
            {
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
                --*this;

                return tmp;
            }

            inline reverse_iterator & operator -- () const
            {
                this->shuffle_next();

                return *this;
            }

            inline reverse_iterator operator -- ( int ) const
            {
                reverse_iterator tmp = *this;
                ++*this;

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
            return iterator( &m_head );
        }

        inline const_iterator begin() const
        {
            return const_iterator( m_head.m_right );
        }

        inline const_iterator end() const
        {
            return const_iterator( &m_head );
        }

        inline reverse_iterator rbegin()
        {
            return reverse_iterator( m_head.m_left );
        }

        inline reverse_iterator rend()
        {
            return reverse_iterator( &m_head );
        }

    public:
        inline T * front() const
        {
            return static_cast<T *>(m_head.m_right);
        }

        inline T * back() const
        {
            return static_cast<T *>(m_head.m_left);
        }

    public:
        inline void push_front( linked_type * _node )
        {
            iterator it = this->begin();

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
            this->erase( _from );
            this->insert( --_where, *_from );
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
            size_type count = stdex::helper::intrusive_distance( it, it_end );

            return count;
        }

        inline bool empty() const
        {
            return this->begin() == this->end();
        }

        inline iterator insert( iterator _where, linked_type * _node )
        {
            this->insert_( _where, _node );

            return (--_where);
        }

        inline void insert( iterator _where, iterator _begin, iterator _end )
        {
            if( _begin == _end )
            {
                return;
            }

            _end->m_left->m_right = nullptr;
            _end->m_left = _begin->m_left;

            _begin->m_left->m_right = *_end;
            _begin->m_left = nullptr;

            _begin->linkall( *_where );
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

        inline void remove( linked_type * _node )
        {
            _node->unlink();
        }

        template<class F>
        inline iterator erase_if( F _pred )
        {
            iterator it_found = find_if( _pred );

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

    protected:
        mutable linked_type m_head;
    };
}