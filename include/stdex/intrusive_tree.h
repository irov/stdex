#	pragma once

namespace stdex
{
	template<class T>
	struct intrusive_tree_node
	{
		intrusive_tree_node()
			: parent(nullptr)
			, left(nullptr)
			, right(nullptr)
		{
		}

		intrusive_tree_node * parent;
		intrusive_tree_node * left;
		intrusive_tree_node * right;


		template<class K>
		struct less_type_pod
		{
			bool operator() ( K _l, K _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_type
		{
			bool operator() ( const K & _l, const K & _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_type<K *>
		{
			bool operator() ( const K * _l, const K * _r ) const
			{
				return _l < _r;
			}
		};
	};

	namespace detail
	{
		template<class T>
		class intrusive_tree_base
		{
		public:
			typedef intrusive_tree_node<T> node_type;

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

		protected:
			void erase_( node_type * z )
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
			template<class T>
			class iterator_base
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

				T * operator -> () const 
				{ 
					return static_cast<T *>(m_node);
				}

				T * operator * () const 
				{ 
					return static_cast<T *>(m_node); 
				}

			protected:
				node_type * m_node;
			};

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
				return nullptr;
			}

		protected:
			mutable node_type * m_root;
		};
	}

	template<class T>
	class intrusive_tree
		: public detail::intrusive_tree_base<T>
	{
	public:
		typedef typename detail::intrusive_tree_base<T>::node_type node_type;

		typedef typename T::key_type key_type;
		typedef typename T::less_type less_type;
		typedef typename T::key_getter_type key_getter_type;

	protected:
		template<class K>
		struct key_getter_type_cast_t
		{
			const K & operator () ( const node_type * _node ) const
			{
				const T * t = static_cast<const T *>(_node);

				return key_getter_type()(t);
			}
		};

		template<class K>
		struct key_getter_type_cast_t<K *>
		{
			K * operator () ( const node_type * _node ) const
			{
				const T * t = static_cast<const T *>(_node);

				return key_getter_type()(t);
			}
		};
		
		typedef key_getter_type_cast_t<key_type> key_getter_type_cast;

	public:
		intrusive_tree()
		{
		}

	public:
		bool insert( T * _node, T ** _out )
		{
			node_type * z = this->getRoot_();
			node_type * p = nullptr;

			bool less_z_node = false;

			while( z != nullptr )
			{
				p = z;

				less_z_node = less_type()( key_getter_type_cast()(z), key_getter_type_cast()(_node) );

				if( less_z_node == true )
				{
					z = z->right;
				}
				else 
				{
					z = z->left;
				}
			}

			if( p == nullptr )
			{
				this->setRoot_( _node );

				if( _out != nullptr )				
				{
					*_out = _node;
				}

				return true;
			}

			if( less_z_node == false && 
				less_type()( key_getter_type_cast()(_node), key_getter_type_cast()(p) ) == false )
			{
				T * node = static_cast<T *>(p);

				if( _out != nullptr )				
				{
					*_out = node;
				}

				return false;
			}

			z = _node;
			z->parent = p;

			if( less_type()( key_getter_type_cast()(p), key_getter_type_cast()(z) ) == true )
			{
				p->right = z;
			}
			else 
			{
				p->left = z;
			}

			if( _out != nullptr )				
			{
				*_out = _node;
			}

			return true;
		}

		bool exist( const key_type & key ) const
		{
			const node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				if( less_type()( key_getter_type_cast()(z), key ) == true )
				{
					z = z->right;
				}
				else if( less_type()( key, key_getter_type_cast()(z) ) == true )
				{
					z = z->left;
				}
				else 
				{
					return true;
				}
			}

			return false;
		}

		bool has( const key_type & key, T ** _out ) 
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return false;
			}

			T * t = static_cast<T *>(z);

			*_out = t;

			return true;
		}

		bool has( const key_type & key, const T ** _out ) const
		{
			const node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return false;
			}

			const T * t = static_cast<const T *>(z);

			*_out = t;

			return true;
		}

		T * find( const key_type & key ) 
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return nullptr;
			}

			T * t = static_cast<T *>(z);

			return t;
		}

		const T * find( const key_type & key ) const
		{
			const node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return nullptr;
			}

			const T * t = static_cast<const T *>(z);

			return t;
		}

	public:
		bool erase( const key_type & key ) 
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return false;
			}

			this->erase_( z );

			return true;
		}

		T * pop( const key_type & key )
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return nullptr;
			}

			this->erase_( z );

			T * t = static_cast<T *>(z);

			return t;
		}

		template<class F>
		void foreach( F f ) const
		{
			node_type * root = this->getRoot_();

			if( root == nullptr )
			{
				return;
			}

			this->foreach_function_node_( root, f );		
		}

		template<class C, class M>
		void foreach( C * _c, M m ) const
		{
			node_type * root = this->getRoot_();

			if( root == nullptr )
			{
				return;
			}

			this->foreach_method_node_( root, _c, m );				
		}

	protected:
		template<class F>
		void foreach_function_node_( node_type * _node, F f ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			T * t = static_cast<T *>(_node);

			f( t );

			if( left != nullptr )
			{
				this->foreach_function_node_( left, f );
			}

			if( right != nullptr )
			{
				this->foreach_function_node_( right, f );
			}
		}

		template<class C, class M>
		void foreach_method_node_( node_type * _node, C * c, M m ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			T * t = static_cast<T *>(_node);

			(c->*m)( t );

			if( left != nullptr )
			{
				this->foreach_method_node_( left, c, m );
			}

			if( right != nullptr )
			{
				this->foreach_method_node_( right, c, m );
			}
		}

	protected:
		node_type * find_node_( const key_type & key )
		{
			node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				if( less_type()( key_getter_type_cast()(z), key ) == true )
				{
					z = z->right;
				}
				else if( less_type()( key, key_getter_type_cast()(z) ) == true )
				{
					z = z->left;
				}
				else 
				{
					return z;
				}
			}

			return nullptr;
		}

		const node_type * find_node_( const key_type & key ) const
		{
			const node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				if( less_type()( key_getter_type_cast()(z), key ) == true )
				{
					z = z->right;
				}
				else if( less_type()( key, key_getter_type_cast()(z) ) == true )
				{
					z = z->left;
				}
				else 
				{
					return z;
				}
			}

			return nullptr;
		}
	};
}