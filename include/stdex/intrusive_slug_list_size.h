#	pragma once

#	include "stdex/intrusive_slug_linked.h"
#	include "stdex/intrusive_algorithm.h"

namespace stdex
{
	template<class T>
	class intrusive_slug_list_size
	{
	public:
		typedef intrusive_slug_linked<T> linked_type;
        typedef T * value_type;

	public:
		intrusive_slug_list_size()
            : m_head(EILT_END)
			, m_size(0)
			, m_slugs(0)
		{
			m_head.m_right = &m_head;
			m_head.m_left = &m_head;
		}

		~intrusive_slug_list_size()
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
				: m_node(_node)
			{
                this->adapt_node();
			}

            inline base_iterator( linked_type * _node, bool _stable )
                : m_node(_node)
            {
                (void)_stable;
            }

            inline base_iterator( const base_iterator & _node )
                : m_node(_node.get())
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
				}
				while( m_node->getIntrusiveTag() == EILT_SLUG );
			}

			inline void shuffle_prev()
			{
				do
				{
					m_node = m_node->left();
				}
				while( m_node->getIntrusiveTag() == EILT_SLUG );			
			}

            inline void adapt_node()
            {
                while( m_node->getIntrusiveTag() == EILT_SLUG )
                {
                    m_node = m_node->right();
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
				: base_iterator<const_iterator>(_node)
			{
			}

            inline const_iterator( linked_type * _node, bool _stable )
                : base_iterator<const_iterator>(_node, _stable)
            {
            }

            inline const_iterator( const const_iterator & _node )
                : base_iterator<const_iterator>(_node)
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

			inline const_iterator operator ++ (int)
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
				: base_iterator<iterator>(_node)
			{
			}

            inline iterator( linked_type * _node, bool _stable)
                : base_iterator<iterator>(_node, _stable)
            {
            }

            inline iterator( const iterator & _node )
                : base_iterator<iterator>(_node)
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

			inline iterator operator ++ (int)
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
				: base_iterator<reverse_iterator>(_node)
			{
			}

            inline reverse_iterator( linked_type * _node, bool _stable )
                : base_iterator<reverse_iterator>(_node, _stable)
            {
            }

            inline reverse_iterator( const reverse_iterator & _node )
                : base_iterator<reverse_iterator>(_node)
            {
            }

        private:
            inline reverse_iterator & operator = ( const reverse_iterator & _it )
            {
                return *this;
            }

        public:
			inline reverse_iterator & operator ++ ()
			{
				this->shuffle_prev();

				return *this;
			}

			inline reverse_iterator operator ++ (int)
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
			return iterator(m_head.m_right);
		}

		inline iterator end()
		{
			return iterator(&m_head, true);
		}

		inline const_iterator begin() const
		{
			return const_iterator(m_head.m_right);
		}

		inline const_iterator end() const
		{
			return const_iterator(&m_head, true);
		}

		inline reverse_iterator rbegin()
		{
			return reverse_iterator(m_head.m_left);
		}

		inline reverse_iterator rend()
		{
			return reverse_iterator(&m_head, true);
		}

	public:
		inline T * front() const
		{
			return *this->begin();
		}

		inline T * back() const
		{
			return *this->end();
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

		inline void clear()
		{
			iterator it = this->begin();
			iterator it_end = this->end();
			while( it != it_end )
			{
				it = this->erase( it );
			}
		}

		inline value_type single() const
		{            
			if( m_size != 1 )
			{
				return nullptr;					 
			}

			if( m_slugs != 0 )
			{
				return nullptr;
			}

			linked_type * first = m_head.right();

			return static_cast<value_type>(first);
		}

		inline size_t size() const
		{            
			return m_size;
		}

		inline bool empty() const
		{
			return m_size == 0;
		}

		inline iterator insert( iterator _where, linked_type * _node )
		{	
			this->insert_( _where, _node );
			
			return (--_where);
		}

		inline iterator erase( iterator _where )
		{	
			iterator it = _where++;

			if( it != this->end() )
			{	
				linked_type * node = it.get();

				this->remove( node );
			}

			return (_where);
		}

		inline void remove( linked_type * _node )
		{
			_node->unlink();
			--m_size;
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

	public:
		void increfSlug()
		{
			++m_slugs;
		}

		void decrefSlug()
		{
			--m_slugs;
		}

		size_t countSlugs() const
		{
			return m_slugs;
		}

		linked_type * head() const
		{
			return &m_head;
		}

	protected:
		inline void insert_( iterator _where, linked_type * _node )
		{	
            linked_type * linked = _where.get();
			linked->link_before( _node );
			++m_size;
		}

	protected:
		mutable linked_type m_head;
		size_t m_size;
		size_t m_slugs;
	};
}