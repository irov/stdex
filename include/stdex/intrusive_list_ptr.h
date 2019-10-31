#pragma once

#include "stdex/intrusive_linked_ptr.h"
#include "stdex/intrusive_algorithm.h"

namespace stdex
{
    template<class T>
    class intrusive_list_ptr
        : public T::value_type
    {
    public:
        typedef uint32_t size_type;
        typedef typename T::value_type value_type;
        typedef typename T::derived_type derived_type;
        typedef T value_type_ptr;

    public:
        intrusive_list_ptr()
        {
            stdex::intrusive_this_acquire( this );

            this->m_head = this;
            this->m_right = this->m_head;
            this->m_left = this->m_head;
        }

        ~intrusive_list_ptr()
        {
            this->clear();

            this->m_head = nullptr;
            this->m_right = nullptr;
            this->m_left = nullptr;
        }

    protected:
        template<class It>
        class base_iterator
        {
        public:
            inline explicit base_iterator( const value_type_ptr & _value )
                : m_value( &_value )
            {
            }

            inline base_iterator( const base_iterator & _it )
                : m_value( _it.get() )
            {
            }

        public:
            inline const value_type_ptr & operator -> () const
            {
                return *m_value;
            }

            inline const value_type_ptr & operator * () const
            {
                return *m_value;
            }

            inline const value_type_ptr * get() const
            {
                return m_value;
            }

        public:
            inline bool operator == ( const It & _it ) const
            {
                const value_type * lptr = m_value->get();
                const value_type * rptr = _it.get()->get();

                return lptr == rptr;
            }

            inline bool operator != ( const It & _it ) const
            {
                return !this->operator == ( _it );
            }

        protected:
            inline void shuffle_next()
            {
                m_value = &(*m_value)->right();
            }

            inline void shuffle_prev()
            {
                m_value = &(*m_value)->left();
            }

        protected:
            const value_type_ptr * m_value;
        };

    public:
        class const_iterator
            : public base_iterator<const_iterator>
        {
        public:
            inline explicit const_iterator( const value_type_ptr & _value )
                : base_iterator<const_iterator>( _value )
            {
            }

            inline const_iterator( const const_iterator & _it )
                : base_iterator<const_iterator>( _it )
            {
            }

        public:
            inline const_iterator & operator = ( const const_iterator & _it )
            {
                this->m_value = _it.m_value;

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
            inline explicit iterator( const value_type_ptr & _value )
                : base_iterator<iterator>( _value )
            {
            }

            inline iterator( const iterator & _it )
                : base_iterator<iterator>( _it )
            {
            }

        public:
            inline iterator & operator = ( const iterator & _it )
            {
                this->m_value = _it.m_value;

                return *this;
            }

        public:
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
            inline explicit reverse_iterator( const value_type_ptr & _value )
                : base_iterator<reverse_iterator>( _value )
            {
            }

            inline reverse_iterator( const reverse_iterator & _it )
                : base_iterator<reverse_iterator>( _it )
            {
            }

        public:
            inline reverse_iterator & operator = ( const reverse_iterator & _it )
            {
                this->m_value = _it.m_value;

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
                -- * this;

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
                ++ * this;

                return tmp;
            }
        };

    public:
        inline iterator begin()
        {
            return iterator( this->m_right );
        }

        inline iterator end()
        {
            return iterator( this->m_head );
        }

        inline const_iterator begin() const
        {
            return const_iterator( this->m_right );
        }

        inline const_iterator end() const
        {
            return const_iterator( this->m_head );
        }

        inline reverse_iterator rbegin()
        {
            return reverse_iterator( this->m_left );
        }

        inline reverse_iterator rend()
        {
            return reverse_iterator( this->m_head );
        }

    public:
        inline const value_type_ptr & front() const
        {
            return this->m_right;
        }

        inline const value_type_ptr & back() const
        {
            return this->m_left;
        }

    public:
        inline void push_front( const value_type_ptr & _node )
        {
            iterator it = this->begin();

            this->insert_( it, _node );
        }

        inline void pop_front()
        {
            iterator it = this->begin();

            this->erase( it );
        }

        inline void push_back( const value_type_ptr & _value )
        {
            iterator it = this->end();
            this->insert_( it, _value );
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

        inline iterator insert( iterator _where, const value_type_ptr & _node )
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
                const value_type_ptr & node = *it;

                this->remove( node );
            }

            return (_where);
        }

        inline void remove( const value_type_ptr & _node )
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
        inline void insert_( iterator _where, const value_type_ptr & _node )
        {
            const value_type_ptr & linked = *_where;
            linked->link_before( _node );
        }

    protected:
        void destroy() override
        {
            //Empty
        }

    protected:
        T m_head;
    };
}