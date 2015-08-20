#   include "allocator_helper.h"

#ifdef __cplusplus
extern "C" {
#endif
	namespace stdex
	{
		//////////////////////////////////////////////////////////////////////////
#	define STATIC_VAR_DECLARE(Name, Type) Type & var_##Name (){ static Type v; return v; }
#	define STATIC_VAR(Name) (var_##Name())
#	define STATIC_VAR_PTR(Name) (&var_##Name())

		//////////////////////////////////////////////////////////////////////////
		STATIC_VAR_DECLARE( pools, stdex_allocator_pools );
#	ifndef STDEX_ALLOCATOR_THREADSAFE_OFF
		STATIC_VAR_DECLARE( pools_threadsafe, stdex_allocator_pools );
		STATIC_VAR_DECLARE( thread_ptr, void * );
		STATIC_VAR_DECLARE( thread_lock, stdex_allocator_thread_lock_t );
		STATIC_VAR_DECLARE( thread_unlock, stdex_allocator_thread_unlock_t );
#	endif
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize()
		{
			STATIC_VAR( pools ).global_memory_use = 0;
		}
		//////////////////////////////////////////////////////////////////////////
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
			void * memory = stdex::s_malloc( STATIC_VAR_PTR( pools ), _size );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free( void * _mem )
		{
			stdex::s_free( STATIC_VAR_PTR( pools ), _mem );
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc( size_t _num, size_t _size )
		{
			size_t full_size = _num * _size;

			void * mem = stdex::s_malloc( STATIC_VAR_PTR( pools ), full_size );

			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc( void * _mem, size_t _size )
		{
			void * memory = stdex::s_realloc( STATIC_VAR_PTR( pools ), _mem, _size );

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
#   endif
		//////////////////////////////////////////////////////////////////////////
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
			size_t global_memory_pools = STATIC_VAR( pools ).global_memory_use;
			
			return global_memory_pools;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryuse()
		{
			size_t memory_pools = stdex::s_memoryuse( STATIC_VAR_PTR( pools ) );

			return memory_pools;
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryinfo( stdex_memory_info_t * _info, size_t _count )
		{
			return stdex::s_memoryinfo( STATIC_VAR_PTR( pools ), _info, _count );
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize()
		{
			stdex::s_finalize( STATIC_VAR_PTR( pools ) );

#	ifndef STDEX_ALLOCATOR_THREADSAFE_OFF
			stdex::s_finalize( STATIC_VAR_PTR( pools_threadsafe ) );
#	endif
		}
		//////////////////////////////////////////////////////////////////////////
#	ifndef STDEX_ALLOCATOR_THREADSAFE_OFF
		//////////////////////////////////////////////////////////////////////////
		static void s_thread_lock()
		{
			if( STATIC_VAR(thread_lock) == nullptr )
			{
				return;
			}

			(*STATIC_VAR( thread_lock ))(STATIC_VAR( thread_ptr ));
		}
		//////////////////////////////////////////////////////////////////////////
		static void s_thread_unlock()
		{
			if( STATIC_VAR( thread_unlock ) == nullptr )
			{
				return;
			}

			(*STATIC_VAR( thread_unlock ))(STATIC_VAR( thread_ptr ));
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock )
		{
			STATIC_VAR( pools_threadsafe ).global_memory_use = 0;

			STATIC_VAR( thread_ptr ) = _ptr;
			STATIC_VAR( thread_lock ) = _lock;
			STATIC_VAR( thread_unlock ) = _unlock;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize_threadsafe()
		{			
			STATIC_VAR( thread_ptr ) = nullptr;
			STATIC_VAR( thread_lock ) = nullptr;
			STATIC_VAR( thread_unlock ) = nullptr;
		}
		//////////////////////////////////////////////////////////////////////////
#	else
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock )
		{
			(void)_ptr;
			(void)_lock;
			(void)_unlock;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize_threadsafe()
		{
		}
		//////////////////////////////////////////////////////////////////////////
#	endif
		//////////////////////////////////////////////////////////////////////////
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
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return stdex_malloc( _size );
#	else
			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_malloc( STATIC_VAR_PTR( pools_threadsafe ), _size );
			STDEX_ALLOCATOR_UNLOCK();
#	endif

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free_threadsafe( void * _mem )
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			stdex_free( _mem );
#	else
			STDEX_ALLOCATOR_LOCK();
			stdex::s_free( STATIC_VAR_PTR( pools_threadsafe ), _mem );
			STDEX_ALLOCATOR_UNLOCK();
#	endif
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc_threadsafe( size_t _num, size_t _size )
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return stdex_calloc( _num, _size );
#	else
			size_t full_size = _num * _size;

			STDEX_ALLOCATOR_LOCK();
			void * mem = stdex::s_malloc( STATIC_VAR_PTR( pools_threadsafe ), full_size );
			STDEX_ALLOCATOR_UNLOCK();

			::memset( mem, 0, full_size );

			return mem;
#	endif
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc_threadsafe( void * _mem, size_t _size )
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return stdex_realloc( _num, _size );
#	else
			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_realloc( STATIC_VAR_PTR( pools_threadsafe ), _mem, _size );
			STDEX_ALLOCATOR_UNLOCK();
#	endif

			return memory;
		}
#   endif
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_globalmemoryuse_threadsafe()
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return 0;
#	else
			size_t memory = STATIC_VAR( pools_threadsafe ).global_memory_use;

			return memory;
#	endif
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryuse_threadsafe()
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return 0;
#	else
			size_t memory = stdex::s_memoryuse( STATIC_VAR_PTR( pools_threadsafe ) );

			return memory;
#	endif
		}
		//////////////////////////////////////////////////////////////////////////
		size_t stdex_allocator_memoryinfo_threadsafe( stdex_memory_info_t * _info, size_t _count )
		{
#	ifdef STDEX_ALLOCATOR_THREADSAFE_OFF
			return 0;
#	else
			return stdex::s_memoryinfo( STATIC_VAR_PTR( pools_threadsafe ), _info, _count );
#	endif
		}
	}
#ifdef __cplusplus
};
#endif
