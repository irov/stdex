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
			if( b == e )
			{
				container_type new_c;
				size_type new_s = (s + 1) * 2;
				new_c.resize( new_s );

				if( s > 0 )
				{
					if( e > b )
					{
						value_type * data = &c[b];
						value_type * end = data + e - b + 1;
						std::copy( data, end, &new_c[0] );
					}
					else
					{
						value_type * data1 = &c[b];
						value_type * end1 = data1 + s - b;
						std::copy( data1, end1, &new_c[0] );

						value_type * data2 = &c[0];
						value_type * end2 = data2 + e;
						std::copy( data2, end2, &new_c[s - b] );
					}
				}

				c.swap( new_c );
				e = s;
				b = 0;
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