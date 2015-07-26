#	pragma once

#   include "stdex/pool.h"
#	include "stdex/memorycopy.h"

#   include <stdint.h>
#   include <memory.h>

//#	define STDEX_ALLOCATOR_DISABLE
//#	define STDEX_ALLOCATOR_NOTHREADSAFE

#   ifndef STDEX_ALLOCATOR_INCLUDE
#	define STDEX_ALLOCATOR_INCLUDE "stdlib.h"
#	endif

#   ifndef STDEX_ALLOCATOR_MALLOC
#   define STDEX_ALLOCATOR_MALLOC malloc
#   endif

#   ifndef STDEX_ALLOCATOR_REALLOC
#   define STDEX_ALLOCATOR_REALLOC realloc
#   endif

#   ifndef STDEX_ALLOCATOR_FREE
#   define STDEX_ALLOCATOR_FREE free
#   endif

#	ifndef STDEX_ALLOCATOR_NOTHREADSAFE
#	define STDEX_ALLOCATOR_THREADSAFE
#	endif

#	ifdef STDEX_ALLOCATOR_THREADSAFE
#	define STDEX_ALLOCATOR_LOCK() stdex::s_thread_lock()
#	define STDEX_ALLOCATOR_UNLOCK() stdex::s_thread_unlock()
#	else
#	define STDEX_ALLOCATOR_LOCK()
#	define STDEX_ALLOCATOR_UNLOCK()
#	endif

#	include STDEX_ALLOCATOR_INCLUDE

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	namespace
	{
		class stdex_pool_allocator
		{
		public:
			inline static void * s_malloc( size_t _size )
			{
				return STDEX_ALLOCATOR_MALLOC( _size );
			}

			inline static void * s_realloc( void * _ptr, size_t _size )
			{
				return STDEX_ALLOCATOR_REALLOC( _ptr, _size );
			}

			inline static void s_free( void * _ptr )
			{
				STDEX_ALLOCATOR_FREE( _ptr );
			}
		};
	}
	//////////////////////////////////////////////////////////////////////////
	typedef uint32_t allocator_size_t;
	//////////////////////////////////////////////////////////////////////////
#	define __STDEX_GET_BYTE(I) value |= ((allocator_size_t)bytes[I]) << (8 * I)
#	define __STDEX_SET_BYTE(I) bytes[I] = (_value >> (8 * I)) & 0xFF;
	//////////////////////////////////////////////////////////////////////////
	inline static allocator_size_t mem_get_value( const void * _mem )
	{
		const allocator_size_t * value = static_cast<const allocator_size_t *>(_mem);

		return *value;

		//const uint8_t * bytes = static_cast<const uint8_t *>(_mem);

		//allocator_size_t value = 0U;
		//__STDEX_GET_BYTE( 0 );
		//__STDEX_GET_BYTE( 1 );
		//__STDEX_GET_BYTE( 2 );
		//__STDEX_GET_BYTE( 3 );

		//return value;
	}
	//////////////////////////////////////////////////////////////////////////
	inline void mem_set_value( void * _mem, allocator_size_t _value )
	{
		allocator_size_t * value = static_cast<allocator_size_t *>(_mem);

		*value = _value;

		//uint8_t * bytes = static_cast<uint8_t *>(_mem);

		//__STDEX_SET_BYTE( 0 );
		//__STDEX_SET_BYTE( 1 );
		//__STDEX_SET_BYTE( 2 );
		//__STDEX_SET_BYTE( 3 );
	}
	//////////////////////////////////////////////////////////////////////////
#	undef __STDEX_GET_BYTE
#	undef __STDEX_SET_BYTE
	//////////////////////////////////////////////////////////////////////////
#   define memory_buf_allign(f) (((f + 3) / 4) * 4)
#   define memory_buf_total(f) (memory_buf_allign(f) + sizeof(allocator_size_t))

#   define pool_to_memory(m) static_cast<void *>(static_cast<uint8_t *>(m) + sizeof(allocator_size_t))

#   define pool_setup_index(m, i) mem_set_value(m, i)

#   define memory_ptr(m) (static_cast<uint8_t *>(m) - sizeof(allocator_size_t))
#   define memory_to_pool(m) static_cast<void *>(memory_ptr(m))
#   define memory_to_index(m) mem_get_value( memory_ptr(m) )
	//////////////////////////////////////////////////////////////////////////
#	define allocator_pool(i) s_p##i
#	define allocator_pool_size(i) s_s##i

#   define allocator_pool_def(i, f, c)\
    const static size_t allocator_pool_size(i) = memory_buf_allign(f);\
    typedef pool<memory_buf_total(f), c, stdex_pool_allocator> t_pool_type_p##i;

	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_loop( function )\
        function(0) \
        function(1) \
        function(2) \
        function(3) \
        function(4) \
        function(5) \
        function(6) \
        function(7) \
        function(8) \
        function(9) \
        function(10) \
        function(11) \
        function(12) \
        function(13) \
        function(14) \
        function(15) \
        function(16) \
        function(17) \
        function(18) \
        function(19)

	allocator_pool_def( 0, 4, 128 );
	allocator_pool_def( 1, 8, 2048 );
	allocator_pool_def( 2, 16, 1024 );
	allocator_pool_def( 3, 24, 512 );
	allocator_pool_def( 4, 32, 512 );
	allocator_pool_def( 5, 48, 256 );
	allocator_pool_def( 6, 64, 256 );
	allocator_pool_def( 7, 92, 256 );
	allocator_pool_def( 8, 128, 256 );
	allocator_pool_def( 9, 192, 256 );
	allocator_pool_def( 10, 256, 256 );
	allocator_pool_def( 11, 384, 256 );
	allocator_pool_def( 12, 512, 32 );
	allocator_pool_def( 13, 768, 32 );
	allocator_pool_def( 14, 1024, 16 );
	allocator_pool_def( 15, 2048, 16 );
	allocator_pool_def( 16, 4096, 4 );
	allocator_pool_def( 17, 8192, 2 );
	allocator_pool_def( 18, 16384, 2 );
	allocator_pool_def( 19, 32768, 2 );
	//////////////////////////////////////////////////////////////////////////
	const static allocator_size_t si_count = 20;
	//////////////////////////////////////////////////////////////////////////
	const static size_t s[] =
	{allocator_pool_size( 0 )
	, allocator_pool_size( 1 )
	, allocator_pool_size( 2 )
	, allocator_pool_size( 3 )
	, allocator_pool_size( 4 )
	, allocator_pool_size( 5 )
	, allocator_pool_size( 6 )
	, allocator_pool_size( 7 )
	, allocator_pool_size( 8 )
	, allocator_pool_size( 9 )
	, allocator_pool_size( 10 )
	, allocator_pool_size( 11 )
	, allocator_pool_size( 12 )
	, allocator_pool_size( 13 )
	, allocator_pool_size( 14 )
	, allocator_pool_size( 15 )
	, allocator_pool_size( 16 )
	, allocator_pool_size( 17 )
	, allocator_pool_size( 18 )
	, allocator_pool_size( 19 )
	};
	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_decl( i )\
    t_pool_type_p##i allocator_pool( i );
	//////////////////////////////////////////////////////////////////////////
	struct stdex_allocator_pools
	{
		allocator_pool_loop( allocator_pool_decl );
		size_t s_global_memory_use;
	};
	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_alloc( i )\
    if( align_size <= allocator_pool_size(i) )\
	    {\
        mem = _pools->allocator_pool(i).alloc_block();\
        pi = i;\
	    } else
	//////////////////////////////////////////////////////////////////////////
	static void * s_malloc( stdex_allocator_pools * _pools, size_t _size )
	{
		if( _size == 0 )
		{
			_size = 1;
		}

		void * mem = nullptr;
		allocator_size_t pi = 0;

		allocator_size_t align_size = memory_buf_allign( _size );

		allocator_pool_loop( allocator_pool_alloc )
		{
			allocator_size_t total_size = memory_buf_total( _size );
			mem = stdex_pool_allocator::s_malloc( total_size );

			if( mem == nullptr )
			{
				return nullptr;
			}

			_pools->s_global_memory_use += (size_t)total_size;

			pi = total_size;
		}

		pool_setup_index( mem, pi );

		void * mem_buff = pool_to_memory( mem );

		return mem_buff;
	}
	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_free(i)\
    if( pi == i )\
	    {\
    _pools->allocator_pool(i).free_block(mem_pool);\
	    } else
	//////////////////////////////////////////////////////////////////////////
	static void s_free( stdex_allocator_pools * _pools, void * _mem )
	{
		if( _mem == nullptr )
		{
			return;
		}

		allocator_size_t pi = memory_to_index( _mem );
		void * mem_pool = memory_to_pool( _mem );

		allocator_pool_loop( allocator_pool_free )
		{
			allocator_size_t global_mem_size = pi;

			_pools->s_global_memory_use -= (size_t)global_mem_size;

			void * global_mem_pool = memory_to_pool( _mem );
			stdex_pool_allocator::s_free( global_mem_pool );
		}
	}
	//////////////////////////////////////////////////////////////////////////
	static void * s_realloc( stdex_allocator_pools * _pools, void * _mem, size_t _size )
	{
		if( _mem == nullptr )
		{
			void * mem = s_malloc( _pools, _size );

			return mem;
		}

		if( _size == 0 )
		{
			s_free( _pools, _mem );

			return nullptr;
		}

		allocator_size_t pi = memory_to_index( _mem );

		if( pi >= si_count )
		{
			allocator_size_t mem_size = pi;

			void * mem_pool = memory_to_pool( _mem );
			allocator_size_t total_size = memory_buf_total( _size );
			void * realloc_mem = stdex_pool_allocator::s_realloc( mem_pool, total_size );

			if( realloc_mem == nullptr )
			{
				return nullptr;
			}

			pool_setup_index( realloc_mem, total_size );

			void * realloc_mem_buff = pool_to_memory( realloc_mem );

			_pools->s_global_memory_use -= (size_t)mem_size;
			_pools->s_global_memory_use += (size_t)total_size;

			return realloc_mem_buff;
		}

		size_t pool_size = s[pi];

		if( pool_size >= _size )
		{
			return _mem;
		}

		void * new_mem = s_malloc( _pools, _size );

		stdex::memorycopy( new_mem, 0, _mem, pool_size );

		s_free( _pools, _mem );

		return new_mem;
	}
	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_info(i) \
	    { \
        _info[i].block_size = _pools->allocator_pool(i).getBlockSize(); \
        _info[i].chunk_count = _pools->allocator_pool(i).getChunkCount(); \
        _info[i].block_count = _pools->allocator_pool(i).getBlockCount(); \
        _info[i].block_total = _pools->allocator_pool(i).getBlockTotal(); \
		++count; \
	    }
	//////////////////////////////////////////////////////////////////////////
	static size_t s_memoryinfo( stdex_allocator_pools * _pools, stdex_memory_info_t * _info, size_t _count )
	{
		if( si_count > _count )
		{
			return 0;
		}

		size_t count = 0;
		allocator_pool_loop( allocator_pool_info );

		return count;
	}
	//////////////////////////////////////////////////////////////////////////
#   define allocator_pool_clear(i) \
			    { \
			_pools->allocator_pool(i).clear(); \
			    }
	//////////////////////////////////////////////////////////////////////////
	static void s_finalize( stdex_allocator_pools * _pools )
	{
		allocator_pool_loop( allocator_pool_clear );
	}
	//////////////////////////////////////////////////////////////////////////
	static size_t s_memoryuse( stdex_allocator_pools * _pools )
	{
		stdex_memory_info_t mi[25];
		size_t count = stdex::s_memoryinfo( _pools, mi, 25 );

		size_t total_now = 0;

		for( size_t i = 0; i != count; ++i )
		{
			total_now += mi[i].block_size * mi[i].block_count;
		}

		total_now += _pools->s_global_memory_use;

		return total_now;
	}
};
