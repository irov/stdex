#   pragma once

#   include <vector>
#   include <algorithm>
#   include <functional>

namespace stdex
{
    template<class Key, class T, class L = std::less<Key> >
    class const_binary_vector
    {
    public:
		typedef std::vector<T> strore_type;
		typedef std::pair<Key, T> binary_store_type;
        typedef std::pair<Key, size_t> binary_pair_type;
        typedef std::vector<binary_pair_type> buffer_type;

        typedef typename buffer_type::iterator iterator;
        typedef typename buffer_type::const_iterator const_iterator;

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
        const_binary_vector()
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
		T & get_value( const_iterator it )
		{
			size_t index = it->second;

			T & value = m_store[index];

			return value;
		}

		const T & get_value( const_iterator it ) const
		{
			size_t index = it->second;

			const T & value = m_store[index];

			return value;
		}

    public:
        std::pair<iterator, bool> insert( const Key & _key, const T & _value )
        {
			size_t index = m_store.size();
			m_store.push_back( _value );

			binary_pair_type bpt = std::make_pair( _key, index );

            iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), bpt, binary_vector_less() );

            if( it_lower_bound != m_buffer.end() )
            {
                if( binary_vector_less()( bpt, *it_lower_bound ) == false )
                {
                    return std::make_pair(it_lower_bound, false);
                }
            }

            iterator it_insert = m_buffer.insert( it_lower_bound, bpt );
            
            return std::make_pair(it_insert, true);
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
        bool has( const Key & _key, T ** _value )
        {
            iterator it_found = this->find( _key );

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

    public:
        iterator find( const Key & _key )
        {
            binary_pair_type key_value(_key, 0);
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
            binary_pair_type key_value(_key, 0);
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
		strore_type m_store;
        buffer_type m_buffer;
    };
}