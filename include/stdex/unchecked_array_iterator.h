#	pragma once

#	include <iterator>

namespace stdex
{
	template<class _Iterator>
	class unchecked_array_iterator
	{	// wrap an iterator (actually, a pointer) without checking, in order to silence warnings
	public:
		typedef unchecked_array_iterator<_Iterator> _Myt;

		typedef typename std::iterator_traits<_Iterator>::iterator_category iterator_category;
		typedef typename std::iterator_traits<_Iterator>::value_type value_type;
		typedef typename std::iterator_traits<_Iterator>::difference_type difference_type;
		typedef typename std::iterator_traits<_Iterator>::difference_type distance_type;	// retained
		typedef typename std::iterator_traits<_Iterator>::pointer pointer;
		typedef typename std::iterator_traits<_Iterator>::reference reference;

		unchecked_array_iterator()
			: _Myptr()
		{	// default construct
		}

		explicit unchecked_array_iterator(_Iterator _Ptr)
			: _Myptr(_Ptr)
		{	// construct with pointer
		}

		_Iterator base() const
		{	// return unwrapped iterator
			return (_Myptr);
		}


		typedef _Iterator _Unchecked_type;

		_Myt& _Rechecked(_Unchecked_type _Right)
		{	// reset from unchecked iterator
			_Myptr = _Right;
			return (*this);
		}

		_Unchecked_type _Unchecked() const
		{	// make an unchecked iterator
			return (base());
		}


		reference operator*() const
		{	// return designated object
			return (*_Myptr);
		}

		pointer operator->() const
		{	// return pointer to class object
			return (&**this);
		}

		_Myt& operator++()
		{	// preincrement
			++_Myptr;
			return (*this);
		}

		_Myt operator++(int)
		{	// postincrement
			_Myt _Tmp = *this;
			++*this;
			return (_Tmp);
		}

		_Myt& operator--()
		{	// predecrement
			--_Myptr;
			return (*this);
		}

		_Myt operator--(int)
		{	// postdecrement
			_Myt _Tmp = *this;
			--*this;
			return (_Tmp);
		}

		_Myt& operator+=(difference_type _Off)
		{	// increment by integer
			_Myptr += _Off;
			return (*this);
		}

		_Myt operator+(difference_type _Off) const
		{	// return this + integer
			_Myt _Tmp = *this;
			return (_Tmp += _Off);
		}

		_Myt& operator-=(difference_type _Off)
		{	// decrement by integer
			return (*this += -_Off);
		}

		_Myt operator-(difference_type _Off) const
		{	// return this - integer
			_Myt _Tmp = *this;
			return (_Tmp -= _Off);
		}

		difference_type operator-(const _Myt& _Right) const
		{	// return difference of iterators
			return (_Myptr - _Right._Myptr);
		}

		reference operator[](difference_type _Off) const
		{	// subscript
			return (*(*this + _Off));
		}

		bool operator==(const _Myt& _Right) const
		{	// test for iterator equality
			return (_Myptr == _Right._Myptr);
		}

		bool operator!=(const _Myt& _Right) const
		{	// test for iterator inequality
			return (!(*this == _Right));
		}

		bool operator<(const _Myt& _Right) const
		{	// test if this < _Right
			return (_Myptr < _Right._Myptr);
		}

		bool operator>(const _Myt& _Right) const
		{	// test if this > _Right
			return (_Right < *this);
		}

		bool operator<=(const _Myt& _Right) const
		{	// test if this <= _Right
			return (!(_Right < *this));
		}

		bool operator>=(const _Myt& _Right) const
		{	// test if this >= _Right
			return (!(*this < _Right));
		}

	private:
		_Iterator _Myptr;	// underlying pointer
	};
}