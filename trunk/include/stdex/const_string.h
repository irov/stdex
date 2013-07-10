#	pragma once

#   include "stdex/const_string_holder.h"
#   include "stdex/const_string_holder_none.h"

#	include "stdex/intrusive_ptr.h"
#	include "stdex/intrusive_linked.h"

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
        typedef size_t size_type;

	public:
        explicit const_string( const_string_holder * _holder )
            : m_holder(_holder)
        {

        }

    public:
        static const const_string & none();

	public:
		inline size_t size() const
		{
			return m_holder->size();
		}

		inline const char * c_str() const
		{
			return m_holder->c_str();
		}

		inline bool empty() const
		{
			return m_holder->empty();
		}

        inline size_t hash() const
        {
            return m_holder->hash();
        }

		inline void clear()
		{
            *this = const_string::none();
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

	public:
        typedef intrusive_ptr<const_string_holder> t_const_string_holder_ptr;
		t_const_string_holder_ptr m_holder;
	};
    //////////////////////////////////////////////////////////////////////////
	bool operator == ( const const_string & _left, const char * _right );
    //////////////////////////////////////////////////////////////////////////
	bool operator == ( const char * _left, const const_string & _right );
}