#	pragma once

#   include "stdex/const_string2_holder.h"
#   include "stdex/const_string2_holder_none.h"

#	include "stdex/intrusive_ptr.h"

namespace stdex
{
	class const_string2
	{
	public:
		const_string2()
			: m_holder(const_string2::none().m_holder)
		{
		}

		const_string2( const const_string2 & _cstr )
			: m_holder(_cstr.m_holder)
		{
		}

	public:
		typedef const_string2_holder holder;

		typedef holder::size_type size_type;
		typedef holder::hash_type hash_type;
        typedef holder::value_type value_type;

	public:
		explicit const_string2( holder * _holder )
			: m_holder(_holder)
		{

		}

	public:
		static const const_string2 & none();

	public:
		inline size_type size() const
		{
			size_type holder_size = m_holder->size();

			return holder_size;
		}

		inline const value_type * c_str() const
		{
			const value_type * holder_str = m_holder->c_str();

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
			*this = const_string2::none();
		}

		inline void swap( const_string2 & _cs )
		{
			m_holder.swap( _cs.m_holder );
		}

	public:
		inline const_string2 & operator = ( const const_string2 & _right )
		{
            m_holder = _right.m_holder;

			return *this;
		}

		inline bool operator == ( const const_string2 & _right ) const
		{			
			return m_holder == _right.m_holder;
		}

		inline bool operator != ( const const_string2 & _right ) const
		{
			return ! this->operator == ( _right );
		}

		inline bool operator < ( const const_string2 & _right ) const
		{
            return m_holder < _right.m_holder;
		}

		inline bool operator <= ( const const_string2 & _right ) const
		{
            return m_holder <= _right.m_holder;
		}

		inline bool operator > ( const const_string2 & _right ) const
		{
			bool result = ! ( *this <= _right );

			return result;
		}

		inline bool operator >= ( const const_string2 & _right ) const
		{
			bool result = ! ( *this < _right );

			return result;
		}

		struct less_type
		{
			bool operator () ( const stdex::const_string2 & _left, const stdex::const_string2 & _right ) const
			{
				return _left < _right;
			}
		};

	protected:
		typedef intrusive_ptr<holder> holder_ptr;
		holder_ptr m_holder;
	};
	//////////////////////////////////////////////////////////////////////////
	bool operator == ( const const_string2 & _left, const const_string2::value_type * _right );
	//////////////////////////////////////////////////////////////////////////
	bool operator == ( const const_string2::value_type * _left, const const_string2 & _right );
	//////////////////////////////////////////////////////////////////////////
	bool operator != ( const const_string2 & _left, const const_string2::value_type * _right );
	//////////////////////////////////////////////////////////////////////////
	bool operator != ( const const_string2::value_type * _left, const const_string2 & _right );
	//////////////////////////////////////////////////////////////////////////
}

namespace std
{
	template<> inline void swap( stdex::const_string2 & _left, stdex::const_string2 & _right )
	{
		_left.swap( _right );
	}
}