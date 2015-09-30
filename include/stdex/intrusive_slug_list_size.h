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
		class base_slug_iterator
		{
		public:
			inline explicit base_slug_iterator( linked_type * _node )
				: m_node(_node)
			{
                this->adapt_node();
			}

            inline explicit base_slug_iterator( linked_type * _node, bool _stable )
                : m_node(_node)
            {
                (void)_stable;
            }

            inline base_slug_iterator( const base_slug_iterator & _it )
                : m_node(_it.get())
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

		public:
			inline bool equal_iterator( const base_slug_iterator & _it ) const
			{
				return m_node == _it.m_node;
			}

			inline linked_type * get() const
			{
				return m_node;
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

		class base_unslug_iterator
		{
		public:
			inline explicit base_unslug_iterator( linked_type * _node )
				: m_node(_node)
			{
			}

			inline explicit base_unslug_iterator( linked_type * _node, bool _stable )
				: m_node(_node)
			{
				(void)_stable;
			}

			inline base_unslug_iterator( const base_unslug_iterator & _it )
				: m_node(_it.get())
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

		public:
			inline bool equal_iterator( const base_unslug_iterator & _it ) const
			{
				return m_node == _it.m_node;
			}

		public:
			inline linked_type * get() const
			{
				return m_node;
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

			inline void adapt_node() const
			{
			}

		protected:
			linked_type * m_node;
		};

	public:
		template<class It>
		class base_iterator
			: public It
		{
		public:
			inline explicit base_iterator( linked_type * _node )
				: It(_node)
			{
			}

            inline base_iterator( linked_type * _node, bool _stable)
                : It(_node, _stable)
            {
            }

            inline base_iterator( const base_iterator & _it )
                : It(_it)
            {
            }

		public:
			inline bool operator == ( base_iterator _it ) const
			{
				return this->equal_iterator( _it );
			}

			inline bool operator != ( base_iterator _it ) const
			{
				return !this->operator == ( _it );
			}

		public:
            inline base_iterator & operator = ( const base_iterator & _it )
            {
                this->m_node = _it.get();

                this->adapt_node();

                return *this;
            }

			inline base_iterator & operator ++ ()
			{
				this->shuffle_next();

				return *this;
			}

			inline base_iterator operator ++ (int)
			{
				base_iterator tmp = *this;
				++*this;
				return tmp;
			}

			inline base_iterator & operator -- ()
			{
				this->shuffle_prev();

				return *this;
			}

			inline base_iterator operator -- ( int )
			{
				base_iterator tmp = *this;
				--*this;
				return tmp;
			}
		};

		template<class It>
		class base_const_iterator
			: public It
		{
		public:
			inline explicit base_const_iterator( linked_type * _node )
				: It(_node)
			{
			}

			inline base_const_iterator( linked_type * _node, bool _stable )
				: It(_node, _stable)
			{
			}

			inline base_const_iterator( const base_const_iterator & _it )
				: It(_it)
			{
			}

		public:
			inline base_const_iterator & operator = ( const base_const_iterator & _it )
			{
				this->m_node = _it.m_node;

				this->adapt_node();

				return *this;
			}

		public:
			inline bool operator == ( base_const_iterator _it ) const
			{
				return this->equal_iterator( _it );
			}

			inline bool operator != ( base_const_iterator _it ) const
			{
				return !this->operator == ( _it );
			}

		public:
			inline base_const_iterator & operator ++ ()
			{
				this->shuffle_next();

				return *this;
			}

			inline base_const_iterator operator ++ (int)
			{
				base_const_iterator tmp = *this;
				++*this;
				return tmp;
			}

			inline base_const_iterator & operator -- ()
			{
				this->shuffle_prev();

				return *this;
			}

			inline base_const_iterator operator -- ( int )
			{
				base_const_iterator tmp = *this;
				--*this;
				return tmp;
			}
		};

		template<class It>
		class base_reverse_iterator
			: public It
		{
		public:
			inline explicit base_reverse_iterator( linked_type * _node )
				: It(_node)
			{
			}

            inline base_reverse_iterator( linked_type * _node, bool _stable )
                : It(_node, _stable)
            {
            }

            inline base_reverse_iterator( const base_reverse_iterator & _it )
                : It(_it)
            {
            }

		public:
			inline bool operator == ( base_reverse_iterator _it ) const
			{
				return this->equal_iterator( _it );
			}

			inline bool operator != ( base_reverse_iterator _it ) const
			{
				return !this->operator == ( _it );
			}

        public:
			inline base_reverse_iterator & operator ++ ()
			{
				this->shuffle_prev();

				return *this;
			}

			inline base_reverse_iterator operator ++ (int)
			{
				base_reverse_iterator tmp = *this;
				--*this;
				return tmp;
			}

			inline base_reverse_iterator & operator -- () const
			{
				this->shuffle_next();

				return *this;
			}

			inline base_reverse_iterator operator -- ( int ) const
			{
				base_reverse_iterator tmp = *this;
				++*this;
				return tmp;
			}
		};

		typedef base_iterator<base_slug_iterator> iterator;
		typedef base_const_iterator<base_slug_iterator> const_iterator;
		typedef base_reverse_iterator<base_slug_iterator> reverse_iterator;

		typedef base_iterator<base_unslug_iterator> unslug_iterator;
		typedef base_const_iterator<base_unslug_iterator> unslug_const_iterator;
		typedef base_reverse_iterator<base_unslug_iterator> unslug_reverse_iterator;		

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
		inline unslug_iterator ubegin()
		{
			return unslug_iterator(m_head.m_right);
		}

		inline unslug_iterator uend()
		{
			return unslug_iterator(&m_head);
		}

		inline unslug_const_iterator ubegin() const
		{
			return unslug_const_iterator(m_head.m_right);
		}

		inline unslug_const_iterator uend() const
		{
			return unslug_const_iterator(&m_head);
		}

		inline unslug_reverse_iterator urbegin()
		{
			return unslug_reverse_iterator(m_head.m_left);
		}

		inline unslug_reverse_iterator urend()
		{
			return unslug_reverse_iterator(&m_head);
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

			iterator it_erase = this->erase( it_found );

			return it_erase;
		}

	public:
		void increfSlug( linked_type * _slug )
		{
			++m_slugs;

			linked_type * linked = this->head();
			linked->link_after( _slug );
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