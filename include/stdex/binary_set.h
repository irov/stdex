#pragma once

#include <vector>
#include <algorithm>

namespace stdex
{
	template<class K, class T, class LK, class L = std::less<K> >
    class binary_set
    {
    public:
        typedef uint32_t size_type;
		typedef std::vector<T> store_type;
		typedef std::vector<size_type> free_type;
        typedef std::vector<size_type> buffer_type;        

        typedef typename buffer_type::iterator iterator;
        typedef typename buffer_type::const_iterator const_iterator;

		typedef std::pair<iterator, bool> inserter_type;

    public:
        binary_set()
        {
        }

    public:
        void reserve( size_type _size )
        {
			m_store.reserve( _size );
            m_buffer.reserve( _size );
        }

        size_type size() const
        {
            buffer_type::size_type size = m_buffer.size();

            return size;
        }

        void clear()
        {
			m_store.clear();
            m_buffer.clear();
			m_free.clear();
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
		const K & get_key( iterator _it ) const
		{
            size_type index = *_it;

			const T & value = m_store[index];

			const K & key = LK()(value);

			return key;
		}

		void set_value( iterator _it, const T & _value )
		{
            size_type index = *_it;

			m_store[index] = _value;
		}

		T & get_value( iterator _it ) const
		{
            size_type index = *_it;

			T & value = m_store[index];

			return value;
		}

		T & get_value( const_iterator _it ) const
		{
            size_type index = *_it;

			T & value = m_store[index];

			return value;
		}

	protected:
        size_type story_value_( const T & _value )
		{
            size_type index;

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

	protected:
		struct binary_set_less_key
		{
		public:
			binary_set_less_key( const store_type & _store )
				: m_store(_store)
			{
			}

		protected:
			void operator = ( const binary_set_less_key & )
			{
			}

		public:
			bool operator () ( size_type _left, const K & _right ) const
			{
				const T & v_l = m_store[_left];

				return L()( LK()(v_l), _right );
			}

		protected:
			const store_type & m_store;
		};

    public:        
        inserter_type insert( const T & _value )
        {
			const K & key = LK()(_value);

            iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), key, binary_set_less_key(m_store) );

            if( it_lower_bound != m_buffer.end() )
            {
				const T & lower_value = this->get_value( it_lower_bound );

				const K & lower_key = LK()(lower_value);

                if( L()( key, lower_key ) == false )
                {
					inserter_type ret = std::make_pair(it_lower_bound, false);

                    return ret;
                }                
            }

            size_type index = this->story_value_( _value );
            
            iterator it_insert = m_buffer.insert( it_lower_bound, index );
            
			inserter_type ret = std::make_pair(it_insert, true);

            return ret;
        }

        void erase( iterator _erase )
        {
            m_buffer.erase( _erase );
        }

        bool erase( const K & _key )
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
        bool has( const K & _key, T ** _value ) const
        {
            const_iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), _key, binary_set_less_key(m_store) );

            const_iterator it_end = this->end();

            if( it_lower_bound == it_end )
            {
                return false;
            }

			T & lower_value = this->get_value( it_lower_bound );

            if( L()( _key, LK()(lower_value) ) == true )
            {
                return false;
            }

			if( _value != nullptr )
			{
				T & value = lower_value;

				*_value = &value;
			}

            return true;
        }

		bool has( const K & _key, const T ** _value ) const
		{
			const_iterator it_lower_bound = std::lower_bound( m_buffer.begin(), m_buffer.end(), _key, binary_set_less_key(m_store) );

			const_iterator it_end = this->end();

			if( it_lower_bound == it_end )
			{
				return false;
			}

			const T & lower_value = this->get_value( it_lower_bound );

			if( L()( _key, LK()(lower_value) ) == true )
			{
				return false;
			}

			if( _value != nullptr )
			{
				const T & value = lower_value;

				*_value = &value;
			}

			return true;
		}

    protected:
		mutable store_type m_store;
        buffer_type m_buffer;
		free_type m_free;
    };
}