#	pragma once

namespace stdex
{
	namespace detail
	{
		template<class T>
		class intrusive_tree_base
		{
		public:
			typedef T node_type;

		public:
			intrusive_tree_base()
				: m_root(nullptr)
			{
			}

		public:
			bool empty() const 
			{ 
				return m_root == nullptr;
			}

			void clear()
			{
				m_root = nullptr;
			}

		protected:
			inline node_type * getRoot_() const
			{
				return m_root;
			}

			inline void setRoot_( node_type * _node ) const
			{
				m_root = _node;
			}

		public:
			void erase_node( node_type * z )
			{
				if( z->left == nullptr )
				{
					this->replace_( z, z->right );
				}
				else if( z->right == nullptr )
				{
					this->replace_( z, z->left );
				}
				else 
				{
					node_type * y = this->subtree_minimum_( z->right );

					if( y->parent != z ) 
					{
						this->replace_( y, y->right );

						y->right = z->right;
						y->right->parent = y;
					}

					this->replace_( z, y );

					y->left = z->left;
					y->left->parent = y;
				}
			}

		protected:
			void replace_( node_type * u, node_type * v ) const
			{
				if( u->parent == nullptr )
				{
					m_root = v;
				}
				else if( u == u->parent->left )
				{
					u->parent->left = v;
				}
				else 
				{
					u->parent->right = v;
				}

				if( v != nullptr )
				{
					v->parent = u->parent;
				}
			}

			node_type * subtree_minimum_( node_type * u ) const
			{
				while( u->left != nullptr )
				{
					u = u->left;
				}

				return u;
			}

		protected:
			class iterator
			{
			public:
				iterator( node_type * _node )
					: m_node(_node)
				{
				}

			public:
				iterator & operator ++ ()
				{
					node_type * parent = m_node->parent;

					if( parent != nullptr && parent->right != nullptr && parent->right != m_node )
					{
						this->max_left_( parent->right );
					}
					else
					{
						m_node = parent;
					}

					return *this;
				}

			protected:
				void max_left_( node_type * _node )
				{
					node_type * tmp = _node;

					for( ;; )
					{
						if( tmp->left != nullptr )
						{
							tmp = tmp->left;
						}
						else if( tmp->right != nullptr )
						{
							tmp = tmp->right;
						}
						else
						{
							break;
						}
					}

					m_node = tmp;
				}

			public:
				bool operator == ( const iterator & _it )
				{
					return m_node == _it.m_node;
				}

				bool operator != ( const iterator & _it )
				{
					return m_node != _it.m_node;
				}

				node_type * operator -> () const 
				{ 
					return m_node;
				}

				node_type * operator * () const 
				{ 
					return m_node; 
				}

			protected:
				node_type * m_node;
			};

			class const_iterator
			{
			public:
				const_iterator( const node_type * _node )
					: m_node(_node)
				{
				}

			public:
				const_iterator & operator ++ ()
				{
					const node_type * parent = m_node->parent;

					if( parent != nullptr && parent->right != nullptr && parent->right != m_node )
					{
						this->max_left_( parent->right );
					}
					else
					{
						m_node = parent;
					}

					return *this;
				}

			protected:
				void max_left_( const node_type * _node )
				{
					const node_type * tmp = _node;

					for( ;; )
					{
						if( tmp->left != nullptr )
						{
							tmp = tmp->left;
						}
						else if( tmp->right != nullptr )
						{
							tmp = tmp->right;
						}
						else
						{
							break;
						}
					}

					m_node = tmp;
				}

			public:
				bool operator == ( const const_iterator & _it )
				{
					return m_node == _it.m_node;
				}

				bool operator != ( const const_iterator & _it )
				{
					return m_node != _it.m_node;
				}

				const node_type * operator -> () const 
				{ 
					return m_node;
				}

				const node_type * operator * () const 
				{ 
					return m_node; 
				}

			protected:
				const node_type * m_node;
			};

		public:
			iterator begin()
			{
				node_type * tmp = m_root;

				for( ;; )
				{
					if( tmp->left != nullptr )
					{
						tmp = tmp->left;
					}
					else if( tmp->right != nullptr )
					{
						tmp = tmp->right;
					}
					else
					{
						break;
					}
				}

				return iterator(tmp);
			}

			iterator end()
			{
				return iterator(nullptr);
			}

			const_iterator begin() const
			{
				const node_type * tmp = m_root;

				for( ;; )
				{
					if( tmp->left != nullptr )
					{
						tmp = tmp->left;
					}
					else if( tmp->right != nullptr )
					{
						tmp = tmp->right;
					}
					else
					{
						break;
					}
				}

				return const_iterator(tmp);
			}

			const_iterator end() const
			{
				return const_iterator(nullptr);
			}

		protected:
			mutable node_type * m_root;
		};
	}
}