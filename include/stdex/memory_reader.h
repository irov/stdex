#	pragma once

#	include <stdint.h>

#	include "stdex/memorycopy.h"

namespace stdex
{
	class memory_reader_exception
	{
	};

	inline void throw_memory_reader_exception()
	{
		throw memory_reader_exception();
	}

	class memory_reader
	{
	public:
		memory_reader( const void * _buff, size_t _capacity, size_t & _read )
			: m_buff(_buff)
			, m_capacity(_capacity)
			, m_read(_read)
		{
		}

	private:
		memory_reader & operator = ( const memory_reader & )
		{
			return *this;
		}

	public:
		template<class T>
		inline void readPOD( T & _t )
		{
			void * buff = (void *)(&_t);
			this->readBuffer( buff, sizeof(T) );
		}

		template<class T>
		inline void readPODs( T * _t, size_t _size )
		{
			void * buff = (void *)(_t);
			this->readBuffer( buff, sizeof(T) * _size );
		}

	public:        
		inline void readBuffer( void * _begin, size_t _size )
		{
			if( m_read + _size > m_capacity )
			{
				throw_memory_reader_exception();
			}

			const unsigned char * read_buff = (const unsigned char *)m_buff + m_read;
			memorycopy( _begin, 0, read_buff, _size );

			m_read += _size;
		}

		inline void readSize( uint32_t & _size )
		{
			uint8_t size_1;
			this->readPOD( size_1 );

			if( size_1 == 255 )
			{
				uint32_t size_2;
				this->readPOD( size_2 );

				_size = size_2;
			}
			else
			{
				_size = (uint32_t)size_1;
			}
		}

	public:
		template<class T>
		inline const T * current_buff() const
		{
			const unsigned char * current = (const unsigned char *)m_buff + m_read;
			const T * buff = reinterpret_cast<const T *>(current);

			return buff;
		}

		inline void skip( size_t _size )
		{
			m_read += _size;
		}

	public:
		template<class T>
		void operator << ( T & _t )
		{
			this->readPOD( _t );
		}

	protected:
		const void * m_buff;
		size_t m_capacity;
		size_t & m_read;
	};
}
