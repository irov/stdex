#	pragma once

#	include "stdex/exception.h"

#	include <stddef.h>

#	ifndef STDEX_INTRUSIVE_PTR_DEBUG_ENABLE
#	ifdef _DEBUG
#	define STDEX_INTRUSIVE_PTR_DEBUG
#	endif
#	else
#	define STDEX_INTRUSIVE_PTR_DEBUG
#	endif

namespace stdex
{
	template<class T> 
	class intrusive_ptr
	{
	public:
		typedef T element_type;

	public:
		inline intrusive_ptr()
			: m_ptr(nullptr)
		{
		}

#	ifndef STDEX_UNSUPPOT_NULLPTR_T
		inline intrusive_ptr( nullptr_t )
			: m_ptr(nullptr)
		{
		}
#	endif

		inline intrusive_ptr( const element_type * _ptr )
			: m_ptr(const_cast<element_type * >(_ptr))
		{
			this->incref();
		}

		template<class U>
		inline intrusive_ptr( const U * _ptr )
			: m_ptr(static_cast<element_type *>(const_cast<U *>(_ptr)))
		{
			this->incref();
		}

		inline intrusive_ptr( const intrusive_ptr & _rhs )
			: m_ptr(_rhs.get())
		{
			this->incref();
		}

		template<class U>
		inline intrusive_ptr( const intrusive_ptr<U> & _rhs )
			: m_ptr(static_cast<T *>(_rhs.get()))
		{
			this->incref();
		}

		inline ~intrusive_ptr()
		{
			this->decref();
		}

		inline intrusive_ptr & operator = ( const intrusive_ptr & _rhs )
		{
			intrusive_ptr swap_ptr(_rhs);
			swap_ptr.swap( *this );

			return *this;
		}

		inline intrusive_ptr & operator = ( element_type * _rhs )
		{
			intrusive_ptr swap_ptr(_rhs);
			swap_ptr.swap( *this );

			return *this;
		}



#	ifndef STDEX_UNSUPPOT_NULLPTR_T
		inline intrusive_ptr & operator = ( nullptr_t )
		{
			intrusive_ptr swap_ptr;
			swap_ptr.swap( *this );

			return *this;
		}
#	endif

	public:
		inline element_type * get() const
		{
			return m_ptr;
		}

		inline element_type * operator -> () const
		{
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
			if( m_ptr == nullptr )
			{
				STDEX_THROW_EXCEPTION("m_ptr == nullptr");
			}

			if( T::intrusive_ptr_check_ref( m_ptr ) == false )
			{
				STDEX_THROW_EXCEPTION("ptr check == false");
			}
#	endif

			return m_ptr;
		}

		inline void swap( intrusive_ptr & _rhs )
		{
			T * tmp = m_ptr;
			m_ptr = _rhs.m_ptr;
			_rhs.m_ptr = tmp;
		}

	protected:
		inline void incref()
		{
			if( m_ptr != nullptr ) 
			{
				T::intrusive_ptr_add_ref( m_ptr );
			}
		}

		inline void decref()
		{
			if( m_ptr != nullptr )
			{
				T::intrusive_ptr_dec_ref( m_ptr );
			}
		}

	protected:
		element_type * m_ptr;
	};
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline void swap( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		_left.swap( _right );
	}
	//////////////////////////////////////////////////////////////////////////
	template<class U, class T>
	inline U intrusive_static_cast( const intrusive_ptr<T> & _iptr )
	{
		typedef typename U::element_type U_type;

		T * t_ptr = _iptr.get();
		U_type * u_ptr = static_cast<U_type *>(t_ptr);

		return U(u_ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class U, class T>
	inline U intrusive_dynamic_cast( const intrusive_ptr<T> & _iptr )
	{
		typedef typename U::element_type U_type;

		T * t_ptr = _iptr.get();
		U_type * u_ptr = dynamic_cast<U_type *>(t_ptr);

		return U(u_ptr);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class U, class T>
	inline U * intrusive_get( const intrusive_ptr<T> & _iptr )
	{
		T * t_ptr = _iptr.get();
		U * u_ptr = static_cast<U *>(t_ptr);

		return u_ptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator < ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		const T * l = _left.get();
		const T * r = _right.get();

		return l < r;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator > ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		const T * l = _left.get();
		const T * r = _right.get();

		return l > r;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator == ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		const T * l = _left.get();
		const T * r = _right.get();

		return l == r;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator != ( const intrusive_ptr<T> & _left, const intrusive_ptr<T> & _right )
	{
		return !(_left == _right);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T, class U> 
	inline bool operator != ( const intrusive_ptr<T> & _left, const U * _right )
	{
		const T * ptr = _left.get();

		return ptr != static_cast<const T *>(_right);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T, class U> 
	inline bool operator != ( const U * _left , const intrusive_ptr<T> & _right )
	{
		const T * ptr = intrusive_get<T>(_right);

		return ptr != static_cast<const T *>(_left);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T, class U> 
	inline bool operator == ( const U * _left , const intrusive_ptr<T> & _right )
	{
		return !(_left != _right);
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T, class U> 
	inline bool operator == ( const intrusive_ptr<T> & _left, const U * _right )
	{
		return !(_left != _right);
	}
	//////////////////////////////////////////////////////////////////////////
#	ifndef STDEX_UNSUPPOT_NULLPTR_T
	template<class T> 
	inline bool operator != ( const intrusive_ptr<T> & _left, nullptr_t )
	{
		const T * ptr = _left.get();

		return ptr != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator != ( nullptr_t , const intrusive_ptr<T> & _right )
	{
		const T * ptr = intrusive_get<T *>(_right);

		return ptr != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator == ( nullptr_t , const intrusive_ptr<T> & _right )
	{
		const T * ptr = intrusive_get<T *>(_right);

		return ptr == nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline bool operator == ( const intrusive_ptr<T> & _left, nullptr_t )
	{
		const T * ptr = _left.get();

		return ptr == nullptr;
	}
#	else
	template<class T> 
	inline bool operator != ( const intrusive_ptr<T> & _left, int )
	{
		const T * ptr = _left.get();

		return ptr != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator != ( int , const intrusive_ptr<T> & _right )
	{
		const T * ptr = intrusive_get<T *>(_right);

		return ptr != nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T> 
	inline bool operator == ( int , const intrusive_ptr<T> & _right )
	{
		const T * ptr = intrusive_get<T *>(_right);

		return ptr == nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline bool operator == ( const intrusive_ptr<T> & _left, int )
	{
		const T * ptr = _left.get();

		return ptr == nullptr;
	}
#	endif
}