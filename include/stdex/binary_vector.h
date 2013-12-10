#   pragma once

#   include <vector>
#   include <algorithm>
#   include <functional>

namespace stdex
{
	template<class Key, class T, class L = std::less<Key> >
	class binary_vector
	{
	public:
		typedef std::vector<size_t> free_type;
		typedef std::vector<T> strore_type;

	public:
		struct binary_value_store_type
		{
			Key key;
			size_t index;
		};

		typedef std::vector<binary_value_store_type> buffer_type;

	public:
		typedef typename buffer_type::iterator iterator;
		typedef typename buffer_type::const_iterator const_iterator;

	public:
		typedef std::pair<iterator, bool> insert_type;

	protected:
		struct binary_vector_less
		{
		public:
			bool operator () ( const binary_value_store_type & _left, const binary_value_store_type & _right ) const
			{
				return L()( _left.key, _right.key );
			}
		};

	public:
		binary_vector()
		{
		}

	public:
		void reserve( size_t _size )
		{
			m_store.reserve( _size );
			m_buffer.reserve( _size );
		}

		size_t size() const
		{
			size_t size = m_buffer.size();

			return size;
		}

		void clear()
		{
			m_store.clear();
			m_buffer.clear();
			m_free.clear();
		}

		bool empty() const
		{
			bool empty = m_buffer.empty();

			return empty;
		}

	public:
		iterator begin()
		{
			iterator it = m_buffer.begin();

			return it;
		}

		iterator end()
		{
			iterator it = m_buffer.end();

			return it;
		}

		const_iterator begin() const
		{
			const_iterator it = m_buffer.begin();

			return it;
		}

		const_iterator end() const
		{
			const_iterator it = m_buffer.end();

			return it;
		}

	public:
		void set_value( iterator _it, const T & _value )
		{
			size_t index = _it->index;

			m_store[index] = _value;
		}

		T & get_value( iterator _it ) const
		{
			size_t index = _it->index;

			T & value = m_store[index];

			return value;
		}

		T & get_value( const_iterator _it ) const
		{
			size_t index = _it->index;

			T & value = m_store[index];

			return value;
		}

		const Key & get_key( const_iterator _it ) const
		{
			return _it->key;
		}

	protected:
		size_t story_value_( const T & _value )
		{
			size_t index;

			if( m_free.empty() == true )
			{
				index = m_store.size();
				m_store.push_back( _value );
			}
			else
			{
				index = m_free.back();
				m_free.pop_back();

				m_store[index] = _value;
			}

			return index;
		}

	public:
		insert_type insert( const Key & _key, const T & _value )
		{
			binary_value_store_type bvst = {_key, 0};
			iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), bvst, binary_vector_less() );

			if( it_lower_bound != m_buffer.end() )
			{
				if( binary_vector_less()( bvst, *it_lower_bound ) == false )
				{
					insert_type ret = std::make_pair(it_lower_bound, false);

					return ret;
				}
			}

			size_t index = this->story_value_( _value );

			bvst.index = index;

			iterator it_insert = m_buffer.insert( it_lower_bound, bvst );

			insert_type ret = std::make_pair(it_insert, true);

			return ret;
		}

		void erase( iterator _it )
		{
			size_t index = _it->index;
			m_free.push_back( index );
			m_store[index] = T();
			m_buffer.erase( _it );
		}

		bool erase( const Key & _key )
		{
			iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			this->erase( it_found );

			return true;
		}

	public:
		bool exist( const Key & _key ) const
		{
			const_iterator it_found = this->find( _key );

			bool result = (it_found == this->end());

			return result;
		}

		bool has( const Key & _key, T ** _value ) const
		{
			const_iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			if( _value != nullptr )
			{
				T & value = this->get_value( it_found );

				*_value = &value;
			}

			return true;
		}
			
		bool has( const Key & _key, const T ** _value ) const
		{
			const_iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			if( _value != nullptr )
			{
				const T & value = this->get_value( it_found );

				*_value = &value;
			}

			return true;
		}

		bool has_copy( const Key & _key, T & _value ) const
		{
			const_iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			const T & value = this->get_value( it_found );

			_value = value;
	
			return true;
		}

	public:
		iterator find( const Key & _key )
		{
			binary_value_store_type key_value = {_key, 0};
			iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), key_value, binary_vector_less() );

			iterator it_end = this->end();
			
			if( it_lower_bound == it_end )
			{
				return it_end;
			}

			if( binary_vector_less()( key_value, *it_lower_bound ) == true )
			{
				return it_end;
			}

			return it_lower_bound;
		}

		const_iterator find( const Key & _key ) const
		{
			binary_value_store_type key_value = {_key, 0};
			const_iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), key_value, binary_vector_less() );

			const_iterator it_end = this->end();

			if( it_lower_bound == it_end )
			{
				return it_end;
			}

			if( binary_vector_less()( key_value, *it_lower_bound ) == true )
			{
				return it_end;
			}

			return it_lower_bound;
		}

		const buffer_type & get_buffer_() const
		{
			return m_buffer;
		}

	protected:		
		buffer_type m_buffer;
		mutable strore_type m_store;
		free_type m_free;
	};

	template<class Key, class T, class L>
	class binary_vector<Key, T *, L>
	{
	public:
		typedef std::pair<Key, T *> binary_pair_type;
		typedef std::vector<binary_pair_type> buffer_type;

		typedef typename buffer_type::iterator iterator;
		typedef typename buffer_type::const_iterator const_iterator;

		typedef std::pair<iterator, bool> insert_type;

	protected:
		struct binary_vector_less
		{
		public:
			bool operator () ( const binary_pair_type & _left, const binary_pair_type & _right ) const
			{
				return L()( _left.first, _right.first );
			}
		};

	public:
		binary_vector()
		{
		}

	public:
		void reserve( size_t _size )
		{
			m_buffer.reserve( _size );
		}

		size_t size() const
		{
			size_t size = m_buffer.size();

			return size;
		}

		bool empty() const
		{
			bool empty = m_buffer.empty();

			return empty;
		}

		void clear()
		{
			m_buffer.clear();
		}

	public:
		T * get_value( iterator _it ) const
		{
			return _it->second;
		}

		T * get_value( const_iterator _it ) const
		{
			return _it->second;
		}

		void set_value( iterator _it, T * _value )
		{
			_it->second = _value;
		}

		const Key & get_key( const_iterator it ) const
		{
			return it->first;
		}

	public:
		iterator begin()
		{
			iterator it = m_buffer.begin();

			return it;
		}

		iterator end()
		{
			iterator it = m_buffer.end();

			return it;
		}

		const_iterator begin() const
		{
			const_iterator it = m_buffer.begin();

			return it;
		}

		const_iterator end() const
		{
			const_iterator it = m_buffer.end();

			return it;
		}

	public:
		insert_type insert( const Key & _key, T * _value )
		{
			binary_pair_type pair = std::make_pair( _key, _value );
			iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), pair, binary_vector_less() );

			if( it_lower_bound != m_buffer.end() )
			{
				if( binary_vector_less()( pair, *it_lower_bound ) == false )
				{
					return std::make_pair(it_lower_bound, false);
				}
			}

			iterator it_insert = m_buffer.insert( it_lower_bound, pair );

			insert_type ret = std::make_pair(it_insert, true);

			return ret;
		}

		void erase( iterator _erase )
		{
			m_buffer.erase( _erase );
		}

		bool erase( const Key & _key )
		{
			iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			this->erase( it_found );

			return true;
		}

	public:
		bool exist( const Key & _key ) const
		{
			const_iterator it_found = this->find( _key );

			bool result = (it_found == this->end());

			return result;
		}

		bool has( const Key & _key, T ** _it ) const
		{
			const_iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			if( _it != nullptr )
			{
				*_it = it_found->second;
			}

			return true;
		}

		bool has( const Key & _key, const T ** _it ) const
		{
			const_iterator it_found = this->find( _key );

			if( it_found == this->end() )
			{
				return false;
			}

			if( _it != nullptr )
			{
				*_it = it_found->second;
			}

			return true;
		}

	public:
		iterator find( const Key & _key )
		{
			binary_pair_type key_value = std::make_pair( _key, (T*)nullptr );
			iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), key_value, binary_vector_less() );

			iterator it_end = this->end();

			if( it_lower_bound == it_end )
			{
				return it_end;
			}

			if( binary_vector_less()( key_value, *it_lower_bound ) == true )
			{
				return it_end;
			}

			return it_lower_bound;
		}

		const_iterator find( const Key & _key ) const
		{
			binary_pair_type key_value = std::make_pair( _key, (T*)nullptr );
			const_iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), key_value, binary_vector_less() );

			const_iterator it_end = this->end();

			if( it_lower_bound == it_end )
			{
				return it_end;
			}

			if( binary_vector_less()( key_value, *it_lower_bound ) == true )
			{
				return it_end;
			}

			return it_lower_bound;
		}

	protected:
		buffer_type m_buffer;
	};
}