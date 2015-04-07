#	pragma once

#	include "stdex/intrusive_tree_base.h"

namespace stdex
{
	template<class T>
	struct intrusive_duplex_tree_node
	{
		intrusive_duplex_tree_node()
			: parent(nullptr)
			, left(nullptr)
			, right(nullptr)
		{
		}

		T * parent;
		T * left;
		T * right;

		template<class K>
		struct less_first_type_pod
		{
			bool operator() ( K _l, K _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_second_type_pod
		{
			bool operator() ( K _l, K _r ) const
			{
				return _l < _r;
			}
		};
		
		template<class K>
		struct less_first_type
		{
			bool operator() ( const K & _l, const K & _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_second_type
		{
			bool operator() ( const K & _l, const K & _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_first_type<K *>
		{
			bool operator() ( const K * _l, const K * _r ) const
			{
				return _l < _r;
			}
		};

		template<class K>
		struct less_second_type<K *>
		{
			bool operator() ( const K * _l, const K * _r ) const
			{
				return _l < _r;
			}
		};
	};

	template<class T>
	class intrusive_duplex_tree
		: public detail::intrusive_tree_base<T>
	{
	public:
		typedef typename detail::intrusive_tree_base<T>::node_type node_type;

		typedef typename detail::intrusive_tree_base<T>::iterator iterator;
		typedef typename detail::intrusive_tree_base<T>::const_iterator const_iterator;

		typedef typename T::key_first_type key_first_type;
		typedef typename T::key_second_type key_second_type;
		typedef typename T::less_first_type less_first_type;
		typedef typename T::less_second_type less_second_type;
		typedef typename T::key_first_getter_type key_first_getter_type;
		typedef typename T::key_second_getter_type key_second_getter_type;

	protected:
		template<class K>
		struct key_first_getter_type_cast_t
		{
			const K & operator () ( const node_type * _node ) const
			{
				return key_first_getter_type()(_node);
			}
		};

		template<class K>
		struct key_second_getter_type_cast_t
		{
			const K & operator () ( const node_type * _node ) const
			{
				return key_second_getter_type()(_node);
			}
		};

		template<class K>
		struct key_first_getter_type_cast_t<K *>
		{
			K * operator () ( const node_type * _node ) const
			{
				return key_first_getter_type()(_node);
			}
		};

		template<class K>
		struct key_second_getter_type_cast_t<K *>
		{
			K * operator () ( const node_type * _node ) const
			{
				return key_second_getter_type()(_node);
			}
		};
		
		typedef key_first_getter_type_cast_t<key_first_type> key_first_getter_type_cast;
		typedef key_second_getter_type_cast_t<key_second_type> key_second_getter_type_cast;


		struct less_duplex_type
		{
			bool operator() ( const key_first_type & _l1, const key_second_type & _l2, const key_first_type & _r1, const key_second_type & _r2 ) const
			{
				if( less_first_type()( _l1, _r1 ) == true )
				{
					return true;
				}
				else if( less_first_type()( _r1, _l1 ) == true )
				{
					return false;
				}

				return less_second_type()( _l2, _r2 );
			}
		};

		struct cmp_duplex_type
		{
			int32_t operator() ( const key_first_type & _l1, const key_second_type & _l2, const key_first_type & _r1, const key_second_type & _r2 ) const
			{
				if( less_first_type()( _l1, _r1 ) == true )
				{
					return -1;
				}
				else if( less_first_type()( _r1, _l1 ) == true )
				{
					return 1;
				}
				else if( less_second_type()( _l2, _r2 ) == true )
				{
					return -1;
				}
				else if( less_second_type()( _r2, _l2 ) == true )
				{
					return 1;
				}

				return 0;				
			}
		};

	public:
		intrusive_duplex_tree()
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

				less_z_node = less_duplex_type()( 
					key_first_getter_type_cast()(z),				
					key_second_getter_type_cast()(z),
					key_first_getter_type_cast()(_node),
					key_second_getter_type_cast()(_node)
					);

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

			if( less_z_node == false )
			{ 
				if( less_duplex_type()( 
					key_first_getter_type_cast()(_node), 
					key_second_getter_type_cast()(_node),
					key_first_getter_type_cast()(p),					 
					key_second_getter_type_cast()(p)
					) == false )
				{
					T * node = static_cast<T *>(p);

					if( _out != nullptr )				
					{
						*_out = node;
					}

					return false;
				}
			}

			z = _node;
			z->parent = p;

			if( less_duplex_type()( 
				key_first_getter_type_cast()(p), 
				key_second_getter_type_cast()(p),
				key_first_getter_type_cast()(z),				 
				key_second_getter_type_cast()(z) 				
				) == true )
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

		bool exist( const key_first_type & _first, const key_second_type & _second ) const
		{
			const node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				int32_t cmp_z_node = cmp_duplex_type()( 
					key_first_getter_type_cast()(z), 
					key_second_getter_type_cast()(z),
					_first, 					 
					_second
					);

				if( cmp_z_node == -1 )
				{
					z = z->right;
				}
				else if( cmp_z_node == 1 )
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

		bool has( const key_first_type & _first, const key_second_type & _second, T ** _out ) 
		{
			node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return false;
			}

			T * t = static_cast<T *>(z);

			*_out = t;

			return true;
		}

		bool has( const key_first_type & _first, const key_second_type & _second, const T ** _out ) const
		{
			const node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return false;
			}

			const T * t = static_cast<const T *>(z);

			*_out = t;

			return true;
		}

		T * find( const key_first_type & _first, const key_second_type & _second ) 
		{
			node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return nullptr;
			}

			T * t = static_cast<T *>(z);

			return t;
		}

		const T * find( const key_first_type & _first, const key_second_type & _second ) const
		{
			const node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return nullptr;
			}

			const T * t = static_cast<const T *>(z);

			return t;
		}

	public:
		bool erase( const key_first_type & _first, const key_second_type & _second ) 
		{
			node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return false;
			}

			this->erase_node( z );

			return true;
		}

		T * pop( const key_first_type & _first, const key_second_type & _second )
		{
			node_type * z = this->find_node_( _first, _second );

			if( z == nullptr )
			{
				return nullptr;
			}

			this->erase_node( z );

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

		template<class C, class M, class P0>
		void foreach( C * _c, M m, const P0 & p0 ) const
		{
			node_type * root = this->getRoot_();

			if( root == nullptr )
			{
				return;
			}

			this->foreach_method_node_( root, _c, m, p0 );				
		}

		template<class M>
		void foreach_self( M m ) const
		{
			node_type * root = this->getRoot_();

			if( root == nullptr )
			{
				return;
			}

			this->foreach_self_method_node_( root, m );
		}

	protected:
		template<class F>
		void foreach_function_node_( node_type * _node, F f ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			if( left != nullptr )
			{
				this->foreach_function_node_( left, f );
			}

			if( right != nullptr )
			{
				this->foreach_function_node_( right, f );
			}

			T * t = static_cast<T *>(_node);

			f( t );
		}

		template<class C, class M>
		void foreach_method_node_( node_type * _node, C * c, M m ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			if( left != nullptr )
			{
				this->foreach_method_node_( left, c, m );
			}

			if( right != nullptr )
			{
				this->foreach_method_node_( right, c, m );
			}

			T * t = static_cast<T *>(_node);

			(c->*m)( t );
		}

		template<class C, class M, class P0>
		void foreach_method_node_( node_type * _node, C * c, M m, const P0 & p0 ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			if( left != nullptr )
			{
				this->foreach_method_node_( left, c, m, p0 );
			}

			if( right != nullptr )
			{
				this->foreach_method_node_( right, c, m, p0 );
			}

			T * t = static_cast<T *>(_node);

			(c->*m)( t, p0 );
		}

		template<class M>
		void foreach_self_method_node_( node_type * _node, M m ) const
		{
			node_type * left = _node->left;
			node_type * right = _node->right;

			if( left != nullptr )
			{
				this->foreach_self_method_node_( left, m );
			}

			if( right != nullptr )
			{
				this->foreach_self_method_node_( right, m );
			}

			T * t = static_cast<T *>(_node);

			(t->*m)();
		}

	protected:
		node_type * find_node_( const key_first_type & _first, const key_second_type & _second )
		{
			node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				int32_t cmp_z_node = cmp_duplex_type()( 
					key_first_getter_type_cast()(z), 
					key_second_getter_type_cast()(z),
					_first, 					 
					_second
					);

				if( cmp_z_node == -1 )
				{
					z = z->right;
				}
				else if( cmp_z_node == 1 )
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

		const node_type * find_node_( const key_first_type & _first, const key_second_type & _second ) const
		{
			const node_type * z = this->getRoot_();

			while( z != nullptr )
			{
				int32_t cmp_z_node = cmp_duplex_type()( 
					key_first_getter_type_cast()(z), 
					key_second_getter_type_cast()(z),
					_first, 					 
					_second
					);

				if( cmp_z_node == -1 )
				{
					z = z->right;
				}
				else if( cmp_z_node == 1 )
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