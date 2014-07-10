#	pragma once

namespace stdex
{
	template<class T>
	struct intrusive_splay_node
	{
		intrusive_splay_node()
			: parent(nullptr)
			, left(nullptr)
			, right(nullptr)
		{
		}

		intrusive_splay_node * parent;
		intrusive_splay_node * left;
		intrusive_splay_node * right;
	};

	namespace detail
	{
		template<class T>
		class intrusive_splay_tree_base
		{
		public:
			typedef intrusive_splay_node<T> node_type;

		public:
			template<bool v>
			struct policy_splay;

			template<>
			struct policy_splay<true>
			{
				void operator () ( const class detail::intrusive_splay_tree_base<T> * _tree, const stdex::intrusive_splay_node<T> * _node ) const
				{
					_tree->splay_( _node );
				}
			};

			template<>
			struct policy_splay<false>
			{
				void operator () ( const class detail::intrusive_splay_tree_base<T> * _tree, const stdex::intrusive_splay_node<T> * _node ) const
				{
					(void)_tree;
					(void)_node;
					//Empty
				}
			};

		public:
			intrusive_splay_tree_base()
				: m_root(nullptr)
				, m_lock(false)
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

			inline void lockSplay_() const
			{
				m_lock = true;
			}

			inline void unlockSplay_() const
			{
				m_lock = false;
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

			void left_rotate_( node_type * x ) const
			{
				node_type * y = x->right;
				x->right = y->left;

				if( y->left != nullptr ) 
				{
					y->left->parent = x;
				}

				y->parent = x->parent;

				if( x->parent == nullptr ) 
				{
					m_root = y;
				}
				else if( x == x->parent->left ) 
				{
					x->parent->left = y;
				}
				else 
				{
					x->parent->right = y;
				}

				y->left = x;
				x->parent = y;
			}

			void right_rotate_( node_type * x ) const
			{
				node_type * y = x->left;
				x->left = y->right;

				if( y->right != nullptr ) 
				{
					y->right->parent = x;
				}

				y->parent = x->parent;

				if( x->parent == nullptr )
				{
					m_root = y;
				}
				else if( x == x->parent->left )
				{
					x->parent->left = y;
				}
				else 
				{
					x->parent->right = y;
				}

				y->right = x;
				x->parent = y;
			}

			void splay_( const node_type * x ) const
			{
				if( m_lock == true )
				{
					return;
				}

				while( x->parent != nullptr)
				{
					if( x->parent->parent == nullptr )
					{
						if( x->parent->left == x ) 
						{
							this->right_rotate_( x->parent );
						}
						else 
						{
							this->left_rotate_( x->parent );
						}
					} 
					else if( x->parent->left == x && x->parent->parent->left == x->parent ) 
					{
						this->right_rotate_( x->parent->parent );
						this->right_rotate_( x->parent );
					} 
					else if( x->parent->right == x && x->parent->parent->right == x->parent ) 
					{
						this->left_rotate_( x->parent->parent );
						this->left_rotate_( x->parent );
					} 
					else if( x->parent->left == x && x->parent->parent->right == x->parent ) 
					{
						this->right_rotate_( x->parent );
						this->left_rotate_( x->parent );
					} 
					else 
					{
						this->left_rotate_( x->parent );
						this->right_rotate_( x->parent );
					}
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
			mutable node_type * m_root;
			mutable bool m_lock;
		};
	}

	template<class T
		, bool enableSplayFind = false
		, bool enableSplayModify = true>
	class intrusive_splay_tree
		: public detail::intrusive_splay_tree_base<T>
	{
	public:
		typedef typename detail::intrusive_splay_tree_base<T>::node_type node_type;
		typedef typename detail::intrusive_splay_tree_base<T>::policy_splay<enableSplayFind> SplayFind;
		typedef typename detail::intrusive_splay_tree_base<T>::policy_splay<enableSplayModify> SplayModify;

		typedef typename T::key_type key_type;
		typedef typename T::less_type less_type;
		typedef typename T::key_getter_type key_getter_type;

	protected:
		struct key_getter_type_cast
		{
			const key_type & operator () ( const node_type * _node ) const
			{
				const T * t = static_cast<const T *>(_node);

				return key_getter_type()(t);
			}
		};

	public:
		intrusive_splay_tree()
		{
		}

	public:
		bool insert( node_type * _node )
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

				return true;
			}

			if( less_z_node == false && 
				less_type()( key_getter_type_cast()(_node), key_getter_type_cast()(p) ) == false )
			{
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

			SplayModify()( this, _node );

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
					SplayFind()( this, z );

					return true;
				}
			}

			return false;
		}

		T * find( const key_type & key ) 
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return nullptr;
			}

			SplayFind()( this, z );

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

			SplayFind()( this, z );

			const T * t = static_cast<const T *>(z);

			return t;
		}

		bool erase( const key_type & key ) 
		{
			node_type * z = this->find_node_( key );

			if( z == nullptr )
			{
				return false;
			}

			SplayModify()( this, z );

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

			SplayModify()( this, z );

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

			this->lockSplay_();

			this->foreach_node_( root, f );

			this->unlockSplay_();				
		}

	protected:
		template<class F>
		void foreach_node_( node_type * _node, F f ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			T * t = static_cast<T *>(_node);

			f( t );

			if( left != nullptr )
			{
				this->foreach_node_( left, f );
			}

			if( right != nullptr )
			{
				this->foreach_node_( right, f );
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