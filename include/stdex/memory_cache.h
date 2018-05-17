#pragma once

#include "stdex/intrusive_linked.h"
#include "stdex/intrusive_list.h"
#include "stdex/template_pool.h"

namespace stdex
{
	class memory_buffer
		: public intrusive_linked<memory_buffer>
	{
	public:
		memory_buffer()
			: m_memory(nullptr)
			, m_lock(false)
		{
		}

	public:
		void * getMemory() const
		{
			return m_memory;
		}

		size_t getSize() const
		{
			return m_size;
		}

	protected:
		void setMemory( void * _memory, size_t _size )
		{
			m_memory = _memory;
			m_size = _size;
		}

	protected:
		void setLock( bool _lock )
		{
			m_lock = _lock;
		}

		bool isLock() const
		{
			return m_lock;
		}

	protected:
		void * m_memory;
		size_t m_size;

		bool m_lock;

		friend class memory_cache;
	};

	class memory_cache
	{
	public:
		memory_cache();
		~memory_cache();

	public:
		void * lock_memory( size_t _size );
		void unlock_memory( void * _memory );

	public:
		void clear();

	protected:
		typedef intrusive_list<memory_buffer> t_list_memory_buffers;
		t_list_memory_buffers m_buffers;

		typedef template_pool<memory_buffer, 16> t_factory_memory_buffers;
		t_factory_memory_buffers m_factoryBuffer;
	};
}