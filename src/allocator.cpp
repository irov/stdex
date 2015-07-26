#   include "allocator_helper.h"

namespace stdex
{
#ifdef __cplusplus
	extern "C" {
#endif
		static stdex_allocator_pools g_pools;
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize()
		{
			g_pools.s_global_memory_use = 0;
		}
#   ifdef STDEX_ALLOCATOR_DISABLE
		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc( size_t _size )
		{
			return stdex::stdex_pool_allocator::s_malloc( _size );
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free( void * _mem )
		{
			stdex::stdex_pool_allocator::s_free( _mem );
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc( size_t _num, size_t _size )
		{
			size_t full_size = _num * _size;
			void * mem = stdex::stdex_pool_allocator::s_malloc( full_size );
			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc( void * _mem, size_t _size )
		{
			return stdex::stdex_pool_allocator::s_realloc( _mem, _size );
		}
		//////////////////////////////////////////////////////////////////////////
#   else
		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc( size_t _size )
		{
			void * memory = stdex::s_malloc( &g_pools, _size );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free( void * _mem )
		{
			stdex::s_free( &g_pools, _mem );
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc( size_t _num, size_t _size )
		{
			size_t full_size = _num * _size;

			void * mem = stdex::s_malloc( &g_pools, full_size );

			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc( void * _mem, size_t _size )
		{
			void * memory = stdex::s_realloc( &g_pools, _mem, _size );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
#   endif
#   define allocator_pool_bound( i )\
	if( _size < stdex::allocator_pool_size(i) )\
		{\
	bound = stdex::allocator_pool_size(i);\
		} else
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_bound( size_t _size )
		{
			size_t bound = 0;
			allocator_pool_loop( allocator_pool_bound )
			{
				return _size + 4;
			}

			return bound;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_globalmemoryuse()
		{
			size_t memory = g_pools.s_global_memory_use;

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryuse()
		{
			size_t memory = stdex::s_memoryuse( &g_pools );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryinfo( stdex_memory_info_t * _info, size_t _count )
		{
			return stdex::s_memoryinfo( &g_pools, _info, _count );
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize()
		{
			stdex::s_finalize( &g_pools );
		}
#ifdef __cplusplus
	};
#endif
}
