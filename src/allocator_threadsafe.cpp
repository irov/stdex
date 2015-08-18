#   include "stdex/allocator.h"

#   include "allocator_helper.h"

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
#ifdef __cplusplus
	extern "C" {
#endif
		//////////////////////////////////////////////////////////////////////////
		static stdex_allocator_pools g_pools_threadsafe;
		//////////////////////////////////////////////////////////////////////////
		static void * g_thread_ptr = nullptr;
		static stdex_allocator_thread_lock_t g_thread_lock = nullptr;
		static stdex_allocator_thread_unlock_t g_thread_unlock = nullptr;
		//////////////////////////////////////////////////////////////////////////
		inline static void s_thread_lock()
		{
			if( stdex::g_thread_lock == nullptr )
			{
				return;
			}

			(*stdex::g_thread_lock)(g_thread_ptr);
		}
		//////////////////////////////////////////////////////////////////////////
		inline static void s_thread_unlock()
		{
			if( stdex::g_thread_unlock == nullptr )
			{
				return;
			}

			(*stdex::g_thread_unlock)(g_thread_ptr);
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock )
		{
			g_pools_threadsafe.global_memory_use = 0;

			stdex::g_thread_ptr = _ptr;
			stdex::g_thread_lock = _lock;
			stdex::g_thread_unlock = _unlock;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize_threadsafe()
		{
			//stdex::s_finalize( &g_pools_threadsafe );

			stdex::g_thread_ptr = nullptr;
			stdex::g_thread_lock = nullptr;
			stdex::g_thread_unlock = nullptr;
		}
#   ifdef STDEX_ALLOCATOR_DISABLE
		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc_threadsafe( size_t _size )
		{
			return stdex::stdex_pool_allocator::s_malloc( _size );
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free_threadsafe( void * _mem )
		{
			stdex::stdex_pool_allocator::s_free( _mem );
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc_threadsafe( size_t _num, size_t _size )
		{
			size_t full_size = _num * _size;
			void * mem = stdex::stdex_pool_allocator::s_malloc( full_size );
			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc_threadsafe( void * _mem, size_t _size )
		{
			return stdex::stdex_pool_allocator::s_realloc( _mem, _size );
		}
		//////////////////////////////////////////////////////////////////////////
#   else
		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc_threadsafe( size_t _size )
		{
			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_malloc( &g_pools_threadsafe, _size );
			STDEX_ALLOCATOR_UNLOCK();

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free_threadsafe( void * _mem )
		{
			STDEX_ALLOCATOR_LOCK();
			stdex::s_free( &g_pools_threadsafe, _mem );
			STDEX_ALLOCATOR_UNLOCK();
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc_threadsafe( size_t _num, size_t _size )
		{
			size_t full_size = _num * _size;

			STDEX_ALLOCATOR_LOCK();
			void * mem = stdex::s_malloc( &g_pools_threadsafe, full_size );
			STDEX_ALLOCATOR_UNLOCK();

			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc_threadsafe( void * _mem, size_t _size )
		{
			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_realloc( &g_pools_threadsafe, _mem, _size );
			STDEX_ALLOCATOR_UNLOCK();

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
		size_t stdex_allocator_bound_threadsafe( size_t _size )
		{
			size_t bound = 0;
			allocator_pool_loop( allocator_pool_bound )
			{
				return _size + 4;
			}

			return bound;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_globalmemoryuse_threadsafe()
		{
			size_t memory = g_pools_threadsafe.global_memory_use;

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryuse_threadsafe()
		{
			size_t memory = stdex::s_memoryuse( &g_pools_threadsafe );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryinfo_threadsafe( stdex_memory_info_t * _info, size_t _count )
		{
			return stdex::s_memoryinfo( &g_pools_threadsafe, _info, _count );
		}
#ifdef __cplusplus
	};
#endif
}
