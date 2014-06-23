#	pragma once

#	include <stdint.h>

#	include "stdex/memorycopy.h"

namespace stdex
{
	class memory_writer_exception
	{
	};

	inline void throw_memory_writer_exception()
	{
		throw memory_writer_exception();
	}

	class memory_writer
	{
	public:
		memory_writer( unsigned char * _buff, size_t _capacity, size_t & _write )
			: m_buff(_buff)
			, m_capacity(_capacity)
			, m_write(_write)
		{
		}

	private:
		memory_writer & operator = ( const memory_writer & )
		{
			return *this;
		}

	public:
		template<class T>
		inline void writePOD( const T & _t )
		{
			const void * buff = (const void *)(&_t);
			this->writeBuffer( buff, sizeof(T) );
		}

		template<class T>
		inline void writePODs( const T * _t, size_t _size )
		{
			const void * buff = (void *)(_t);
			this->writeBuffer( buff, sizeof(T) * _size );
		}

	public:        
		inline void writeBuffer( const void * _begin, size_t _size )
		{
			if( m_write + _size > m_capacity )
			{
				throw_memory_writer_exception();
			}

			unsigned char * write_buff = m_buff + m_write;
			memorycopy( write_buff, _begin, _size );

			m_write += _size;
		}

		inline void writeSize( uint32_t _size )
		{
			if( _size < 255 )
			{
				uint8_t size_1 = (uint8_t)_size;
				this->writePOD( size_1 );
			}
			else
			{
				uint8_t size_1 = 255;
				this->writePOD( size_1 );

				this->writePOD( _size );
			}
		}

	public:
		template<class T>
		inline T * current_buff() const
		{
			unsigned char * current = m_buff + m_write;
			T * buff = reinterpret_cast<T *>(current);

			return buff;
		}

		inline void skip( size_t _size )
		{
			m_write += _size;
		}

	public:
		template<class T>
		void operator << ( const T & _t )
		{
			this->writePOD( _t );
		}

	protected:
		unsigned char * m_buff;
		size_t m_capacity;
		size_t & m_write;
	};
}
