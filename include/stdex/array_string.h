#	pragma once

#	include "stdex/const_string.h"
#	include "stdex/memorycopy.h"
#	include "stdex/stl_string.h"

#	include <memory.h>
#	include <string.h>

namespace stdex
{
	template<size_t Size>
	class array_string
	{
	public:
		array_string()
			: m_pos(0)
		{
		}

	public:
		const char * c_str() const
		{
			return m_buffer;
		}

		size_t size() const
		{
			return m_pos;
		}

	public:
		void clear()
		{
			m_pos = 0;
		}

		void append( const char * _value, size_t _size )
		{			
			if( m_pos + _size >= Size )
			{
				return;
			}

			memorycopy( m_buffer, m_pos, _value, _size );

			m_pos += _size;
			m_buffer[m_pos] = 0;
		}

		void append( const char * _value )
		{
			size_t size = strlen(_value);

			this->append( _value, size );
		}

		void append( const stdex::const_string & _value )
		{
			const char * value_str = _value.c_str();
			size_t value_size = _value.size();

			this->append( value_str, value_size );
		}

		void append( const stdex::string & _value )
		{
			const char * value_str = _value.c_str();
			size_t value_size = _value.size();

			this->append( value_str, value_size );
		}

		void append( char _ch )
		{
			if( m_pos + 1 >= Size )
			{
				return;
			}

			m_buffer[m_pos] = _ch;
			m_pos += 1;
			m_buffer[m_pos] = 0;
		}

	public:
		void replace_last( const char * _value )
		{
			size_t size = strlen( _value );

			if( size > m_pos )
			{
				return;
			}

			memorycopy( m_buffer, m_pos - size, _value, size );
		}

		void cut_before_last_of( char _ch )
		{
			char * ch_pos = strrchr( m_buffer, _ch );

			if( ch_pos == nullptr )
			{
				return;
			}

			m_pos = ch_pos - m_buffer;
		}

	public:
		void operator += ( const char * _value )
		{
			this->append( _value );
		}

		void operator += ( const stdex::const_string & _value )
		{
			this->append( _value );
		}

		void operator += ( const stdex::string & _value )
		{
			this->append( _value );
		}

		void operator += ( char _value )
		{
			this->append( _value );
		}

	protected:
		char m_buffer[Size];
		size_t m_pos;
	};
}