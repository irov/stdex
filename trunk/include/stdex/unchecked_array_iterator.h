#	pragma once

#	include <iterator>

namespace stdex
{
	template<class T>
	class unchecked_array_iterator
	{
	public:
		typedef T unchecked_type;
		typedef unchecked_array_iterator<T> my_type;		

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

		typedef T _Unchecked_type;

		my_type & _Rechecked( unchecked_type _right )
		{	// reset from unchecked iterator
			m_ptr = _right;
			return (*this);
		}

		unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (base());
		}

		T base() const
		{
			return (m_ptr);
		}
	
		reference operator * () const
		{
			return *m_ptr;
		}

		pointer operator -> () const
		{
			return &**this;
		}

		my_type & operator ++ ()
		{
			++m_ptr;

			return *this;
		}

		my_type operator ++ (int)
		{
			my_type tmp = *this;

			++*this;

			return tmp;
		}

		my_type & operator -- ()
		{
			--m_ptr;

			return *this;
		}

		my_type operator -- (int)
		{
			my_type tmp = *this;
			--*this;

			return tmp;
		}

		my_type & operator += ( difference_type _offset )
		{
			m_ptr += _offset;

			return *this;
		}

		my_type operator + ( difference_type _offset ) const
		{
			my_type tmp = *this;

			return tmp += _offset;
		}

		my_type & operator -= ( difference_type _offset )
		{
			return *this += -_offset;
		}

		my_type operator - ( difference_type _offset ) const
		{
			my_type tmp = *this;

			return tmp -= _offset;
		}

		difference_type operator - ( const my_type & _right ) const
		{
			return m_ptr - _right.m_ptr;
		}

		reference operator [] ( difference_type _index ) const
		{
			return *(*this + _index);
		}

		bool operator == ( const my_type & _right ) const
		{
			return m_ptr == _right.m_ptr;
		}

		bool operator != ( const my_type & _right) const
		{
			return !(*this == _right);
		}

		bool operator < ( const my_type & _right ) const
		{
			return m_ptr < _right.m_ptr;
		}

		bool operator > ( const my_type & _right ) const
		{
			return _right < *this;
		}

		bool operator <= ( const my_type & _right ) const
		{
			return !(_right < *this);
		}

		bool operator >= ( const my_type & _right ) const
		{
			return !(*this < _right);
		}

	private:
		T m_ptr;
	};
}