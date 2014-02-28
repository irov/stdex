#	include "stdex/memory_cache.h"

#	include <malloc.h>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	memory_cache::memory_cache()
	{
	}
	//////////////////////////////////////////////////////////////////////////
	memory_cache::~memory_cache()
	{
		this->clear();
	}
	//////////////////////////////////////////////////////////////////////////
	void * memory_cache::lock_memory( size_t _size )
	{
		size_t minSize = (size_t)(0);
		size_t maxSize = (size_t)(-1);

		memory_buffer * minBuffer = nullptr;
		memory_buffer * maxBuffer = nullptr;
		
		for( t_list_memory_buffers::iterator
			it = m_buffers.begin(),
			it_end = m_buffers.end();
		it != it_end;
		++it )
		{
			memory_buffer * buffer = *it;

			if( buffer->isLock() == true )
			{
				continue;
			}

			size_t buffer_size = buffer->getSize();

			if( buffer_size > minSize && buffer_size <= _size )
			{
				minSize = buffer_size;
				minBuffer = buffer;
			}

			if( buffer_size < maxSize && buffer_size >= _size )
			{
				maxSize = buffer_size;
				maxBuffer = buffer;
			}
		}
		
		memory_buffer * currentBuffer = nullptr;

		if( maxBuffer != nullptr )
		{
			currentBuffer = maxBuffer;
		}
		else if( minBuffer != nullptr )
		{
			currentBuffer = minBuffer;

			void * memory = minBuffer->getMemory();
			void * realloc_memory = realloc( memory, _size );

			minBuffer->setMemory( realloc_memory, _size );
		}
		else
		{
			currentBuffer = m_factoryBuffer.createT();

			void * memory = malloc( _size );
			currentBuffer->setMemory( memory, _size );

			m_buffers.push_back( currentBuffer );
		}

		currentBuffer->setLock( true );

		void * memory = currentBuffer->getMemory();

		return memory;
	}
	//////////////////////////////////////////////////////////////////////////
	void memory_cache::unlock_memory( void * _memory )
	{
		for( t_list_memory_buffers::iterator
			it = m_buffers.begin(),
			it_end = m_buffers.end();
		it != it_end;
		++it )
		{
			memory_buffer * buffer = *it;

			void * memory = buffer->getMemory();

			if( memory != _memory )
			{
				continue;
			}
			
			buffer->setLock( false );
			break;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	void memory_cache::clear()
	{
		for( t_list_memory_buffers::iterator
			it = m_buffers.begin(),
			it_end = m_buffers.end();
		it != it_end;
		++it )
		{
			memory_buffer * buffer = *it;

			void * memory = buffer->getMemory();
			free( memory );			
		}

		m_buffers.clear();
	}
}