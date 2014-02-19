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
			: b(0)
			, e(0)
			, s(0)
			, n(0)
		{
		}

	public:
		void push( const value_type& _value )
		{
			if( n == s )
			{
				container_type new_c;
				size_type new_s = (s + 1) * 2;
				c.resize( new_s );

				if( b >= e && e != 0 )
				{					
					size_type replace_n = s - b;
					for( size_type i = 0; i != replace_n; i++ )
					{
						size_t l = s - i - 1; 
						size_t k = new_s - i - 1;

						std::swap( c[l], c[k] );
					}

					b = new_s - s + b;
				}
				else
				{
					e = n;
					b = 0;
				}

				s = new_s;
			}

			size_type l = (s + e) % s;
			c[l] = _value;
			e += 1;
			e %= s;
			++n;
		}

		size_type size() const
		{
			return n;
		}

		bool empty() const
		{
			return n == 0;
		}

		reference pop()
		{	// erase element at end
			size_type ret_b = b;

			b += 1;
			b %= s;
			--n;
			
			return c[ret_b];
		}

		void clear()
		{
			c.clear();
			s = 0;
			n = 0;
			b = 0;
			e = 0;
		}

	protected:		
		container_type c;
		size_type s;
		size_type n;
		size_type b;
		size_type e;
	};
}