#	pragma once

#	include <algorithm>

namespace stdex
{	
	template<class T>
	class unchecked_array_iterator
	{
	public:
		typedef typename std::iterator_traits<T>::iterator_category iterator_category;
		typedef typename std::iterator_traits<T>::value_type value_type;
		typedef typename std::iterator_traits<T>::difference_type difference_type;
		typedef typename std::iterator_traits<T>::difference_type distance_type;
		typedef typename std::iterator_traits<T>::pointer pointer;
		typedef typename std::iterator_traits<T>::reference reference;

		unchecked_array_iterator()
			: m_ptr()
		{
		}

		explicit unchecked_array_iterator( T _ptr )
			: m_ptr(_ptr)
		{
		}

		T base() const
		{
			return m_ptr;
		}


		typedef T _Unchecked_type;

		unchecked_array_iterator & _Rechecked( T _ptr )
		{
			m_ptr = _ptr;

			return *this;
		}

		T _Unchecked() const
		{
			return this->base();
		}


		reference operator * () const
		{
			return *m_ptr;
		}

		pointer operator -> () const
		{
			return &**this;
		}

		unchecked_array_iterator & operator++()
		{
			++m_ptr;

			return *this;
		}

		unchecked_array_iterator operator ++ (int)
		{
			unchecked_array_iterator tmp = *this;
			++*this;

			return tmp;
		}

		unchecked_array_iterator & operator--()
		{
			--m_ptr;

			return *this;
		}

		unchecked_array_iterator operator -- (int)
		{
			unchecked_array_iterator tmp = *this;

			--*this;

			return tmp;
		}

		unchecked_array_iterator & operator += ( difference_type _offset )
		{
			m_ptr += _offset;

			return *this;
		}

		unchecked_array_iterator operator + ( difference_type _offset ) const
		{	
			unchecked_array_iterator tmp = *this;
			tmp += _offset;

			return tmp;
		}

		unchecked_array_iterator& operator -= ( difference_type _offset )
		{
			return *this += (-_offset);
		}

		unchecked_array_iterator operator - ( difference_type _offset ) const
		{
			unchecked_array_iterator tmp = *this;
			tmp -= _offset;

			return tmp;
		}

		difference_type operator - ( const unchecked_array_iterator & _it ) const
		{
			return m_ptr - _it.m_ptr;
		}

		reference operator [] ( difference_type _offset ) const
		{
			return *(*this + _offset);
		}

		bool operator == (const unchecked_array_iterator & _it ) const
		{
			return m_ptr == _it.m_ptr;
		}

		bool operator != (const unchecked_array_iterator & _it ) const
		{	
			return !(*this == _it);
		}

		bool operator < (const unchecked_array_iterator & _it ) const
		{
			return m_ptr < _it.m_ptr;
		}

		bool operator > (const unchecked_array_iterator & _it ) const
		{
			return _it < *this;
		}

		bool operator <= ( const unchecked_array_iterator & _it ) const
		{
			return !(_it < *this);
		}

		bool operator >= ( const unchecked_array_iterator & _it ) const
		{
			return !(*this < _it);
		}

	private:
		T m_ptr;
	};

	inline void memorycopy( void * _dist, const void * _src, size_t _size )
	{
		unchecked_array_iterator<unsigned char *> chkd_dist((unsigned char *)_dist);
		std::copy( (unsigned char *)_src, (unsigned char *)_src + _size, chkd_dist );
	}
}
