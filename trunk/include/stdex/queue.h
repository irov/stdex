#   pragma once

#   include <vector>

namespace stdex
{
	template<class T>
	class queue
	{
	public:
		typedef std::vector<T> container_type;
		typedef typename container_type::size_type size_type;
		typedef typename container_type::value_type value_type;
		typedef typename container_type::reference reference;		

	public:
		queue()
			: m_begin(0)
			, m_end(0)
			, m_size(0)
			, m_count(0)
		{
		}

	public:
		void push( const value_type& _value )
		{
			if( m_count == m_size )
			{
				container_type new_c;
				size_type new_s = (m_size + 1) * 2;
				m_container.resize( new_s );

				if( m_begin >= m_end && m_end != 0 )
				{					
					size_type replace_n = m_size - m_begin;
					for( size_type i = 0; i != replace_n; i++ )
					{
						size_t l = m_size - i - 1; 
						size_t k = new_s - i - 1;

						std::swap( m_container[l], m_container[k] );
					}

					m_begin = new_s - m_size + m_begin;
				}
				else
				{
					m_end = m_count;
					m_begin = 0;
				}

				m_size = new_s;
			}

			size_type l = (m_size + m_end) % m_size;
			m_container[l] = _value;
			m_end += 1;
			m_end %= m_size;
			++m_count;
		}

		size_type size() const
		{
			return m_count;
		}

		bool empty() const
		{
			return m_count == 0;
		}

		reference pop()
		{	// erase element at end
			size_type ret_b = m_begin;

			m_begin += 1;
			m_begin %= m_size;
			--m_count;
			
			return m_container[ret_b];
		}

		void clear()
		{
			m_container.clear();
			m_size = 0;
			m_count = 0;
			m_begin = 0;
			m_end = 0;
		}

	protected:		
		container_type m_container;
		size_type m_size;
		size_type m_count;
		size_type m_begin;
		size_type m_end;
	};
}