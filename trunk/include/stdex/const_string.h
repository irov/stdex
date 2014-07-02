#	pragma once

#   include "stdex/const_string_holder.h"
#   include "stdex/const_string_holder_none.h"

#	include "stdex/intrusive_ptr.h"
#	include "stdex/intrusive_linked.h"

#	include <algorithm>

namespace stdex
{
	class const_string
	{
	public:
		const_string()
			: m_holder(const_string::none().m_holder->owner())
		{
		}

		const_string( const const_string & _cstr )
			: m_holder(_cstr.m_holder->owner())
		{
		}

	public:
		typedef const_string_holder holder;

		typedef holder::size_type size_type;
		typedef holder::hash_type hash_type;		

	public:
		explicit const_string( holder * _holder )
			: m_holder(_holder)
		{

		}

	public:
		static const const_string & none();

	public:
		inline size_type size() const
		{
			size_type holder_size = m_holder->size();

			return holder_size;
		}

		inline const char * c_str() const
		{
			const char * holder_str = m_holder->c_str();

			return holder_str;
		}

		inline bool empty() const
		{
			bool holder_empty = m_holder->empty();

			return holder_empty;
		}

		inline hash_type hash() const
		{
			hash_type holder_hash = m_holder->hash();

			return holder_hash;
		}

		inline void clear()
		{
			*this = const_string::none();
		}

		inline void swap( const_string & _cs )
		{
			m_holder.swap( _cs.m_holder );
		}

	public:
		inline const_string & operator = ( const const_string & _right )
		{
			const_string_holder * right_holder = _right.m_holder.get();

			if( m_holder->equal( right_holder ) == false )
			{
				m_holder = _right.m_holder->owner();
			}

			return *this;
		}

		inline bool operator == ( const const_string & _right ) const
		{
			const_string_holder * right_holder = _right.m_holder.get();

			bool result = m_holder->equal( right_holder );

			return result;
		}

		inline bool operator != ( const const_string & _cstr ) const
		{
			return ! this->operator == ( _cstr );
		}

		inline bool operator < ( const const_string & _cstr ) const
		{
			const_string_holder * right_holder = _cstr.m_holder.get();

			bool result = m_holder->less( right_holder );

			return result;
		}

		inline bool operator <= ( const const_string & _cstr ) const
		{
			if( *this < _cstr )
			{
				return true;
			}

			const_string_holder * right_holder = _cstr.m_holder.get();

			bool result = m_holder->equal( right_holder );

			return result;
		}

		inline bool operator > ( const const_string & _right ) const
		{
			bool result = ! ( *this <= _right );

			return result;
		}

		inline bool operator >= ( const const_string & _right ) const
		{
			bool result = ! ( *this < _right );

			return result;
		}

		struct less_type
		{
			bool operator () ( const stdex::const_string & _left, const stdex::const_string & _right )
			{
				return _left < _right;
			}
		};

	protected:
		typedef intrusive_ptr<holder> holder_ptr;
		holder_ptr m_holder;
	};
	//////////////////////////////////////////////////////////////////////////
	bool operator == ( const const_string & _left, const char * _right );
	//////////////////////////////////////////////////////////////////////////
	bool operator == ( const char * _left, const const_string & _right );
	//////////////////////////////////////////////////////////////////////////
}

namespace std
{
	template<> inline void swap( stdex::const_string & _left, stdex::const_string & _right )
	{
		_left.swap( _right );
	}
}