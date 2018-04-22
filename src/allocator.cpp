#include "stdex/allocator.h"
#include "stdex/allocator_report.h"
#include "stdex/pool2.h"
#include "stdex/memorycopy.h"
#include "stdex/exception.h"

#include <stdint.h>
#include <stdlib.h>
#include <memory.h>

//#ifndef NDEBUG
//#   define STDEX_ALLOCATOR_DISABLE
//#endif

#ifndef STDEX_ALLOCATOR_INCLUDE
#	define STDEX_ALLOCATOR_INCLUDE "stdlib.h"
#endif

#ifndef STDEX_ALLOCATOR_MALLOC
#   define STDEX_ALLOCATOR_MALLOC malloc
#endif

#ifndef STDEX_ALLOCATOR_REALLOC
#   define STDEX_ALLOCATOR_REALLOC realloc
#endif

#ifndef STDEX_ALLOCATOR_FREE
#   define STDEX_ALLOCATOR_FREE free
#endif

#ifndef STDEX_ALLOCATOR_NOTHREADSAFE
#	define STDEX_ALLOCATOR_THREADSAFE
#endif

#ifdef STDEX_ALLOCATOR_THREADSAFE
#	define STDEX_ALLOCATOR_LOCK() stdex::s_thread_lock()
#	define STDEX_ALLOCATOR_UNLOCK() stdex::s_thread_unlock()
#else
#	define STDEX_ALLOCATOR_LOCK()
#	define STDEX_ALLOCATOR_UNLOCK()
#endif

#include STDEX_ALLOCATOR_INCLUDE

#ifdef __cplusplus
extern "C" {
#endif
	namespace stdex
	{
		//////////////////////////////////////////////////////////////////////////
		namespace detail
		{
			class stdex_pool_allocator
			{
			public:
				inline static void * s_malloc( size_t _size, const char * _doc )
				{
                    stdex_allocator_report_new( _size, _doc );

					return STDEX_ALLOCATOR_MALLOC( _size );
				}

                inline static void s_free( void * _ptr, const char * _doc )
				{
                    stdex_allocator_report_free( _ptr, _doc );

					STDEX_ALLOCATOR_FREE( _ptr );
				}

                inline static void * s_realloc( void * _ptr, size_t _size, const char * _doc )
                {
                    stdex_allocator_report_realoc( _ptr, _size, _doc );

                    return STDEX_ALLOCATOR_REALLOC( _ptr, _size );
                }
			};
		}
        //////////////////////////////////////////////////////////////////////////
        typedef uint32_t allocator_size_t;
        //////////////////////////////////////////////////////////////////////////
#define __STDEX_GET_BYTE(I) value |= ((allocator_size_t)bytes[I]) << (8 * I)
#define __STDEX_SET_BYTE(I) bytes[I] = (_value >> (8 * I)) & 0xFF;
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
#undef __STDEX_GET_BYTE
#undef __STDEX_SET_BYTE
//////////////////////////////////////////////////////////////////////////
#define memory_buf_align(f) (((f + 3) / 4) * 4)
#define memory_buf_total(f) (memory_buf_align(f) + sizeof(allocator_size_t))
#define pool_to_memory(m) static_cast<void *>(static_cast<uint8_t *>(m) + sizeof(allocator_size_t))
#define memory_to_pool(m) static_cast<void *>(static_cast<uint8_t *>(m) - sizeof(allocator_size_t))
#define pool_setup_index(m, i) mem_set_value(m, i)
#define memory_to_index(m) mem_get_value( memory_to_pool(m) )
//////////////////////////////////////////////////////////////////////////
#define allocator_pool(i) s_p##i
#define allocator_pool_size(i) s_s##i
#define allocator_pool_totalsize(i) s_ts##i

#define allocator_pool_def(i, f, c)\
    const static allocator_size_t allocator_pool_size(i) = memory_buf_align(f);\
    const static allocator_size_t allocator_pool_totalsize(i) = memory_buf_total(f);\
    typedef pool2<memory_buf_total(f), c, detail::stdex_pool_allocator> t_pool_type_p##i;

		//////////////////////////////////////////////////////////////////////////
#define allocator_pool_loop( function )\
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
		const static allocator_size_t s[] =
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
#define allocator_pool_decl( i )\
    static t_pool_type_p##i allocator_pool( i );
		//////////////////////////////////////////////////////////////////////////
		allocator_pool_loop( allocator_pool_decl );
		//////////////////////////////////////////////////////////////////////////
		static bool s_initialize = false;
		static void * s_thread_ptr = nullptr;
		static stdex_allocator_thread_lock_t s_thread_lock_func = nullptr;
		static stdex_allocator_thread_unlock_t s_thread_unlock_func = nullptr;
		//////////////////////////////////////////////////////////////////////////
#ifndef NDEBUG
#	define SDTEX_ALLOCATOR_INITIALIZE_CHECK\
		if( s_initialize == false ){STDEX_THROW_EXCEPTION("stdex allocator not initialize");}
#else
#	define SDTEX_ALLOCATOR_INITIALIZE_CHECK
#endif		
		//////////////////////////////////////////////////////////////////////////
#define allocator_pool_alloc( i )\
    if( align_size <= allocator_pool_size(i) )\
			    {\
        mem = allocator_pool(i).alloc_block();\
        pi = i;\
			    } else
		//////////////////////////////////////////////////////////////////////////
		static void * s_malloc( size_t _size, const char * _doc )
		{
			if( _size == 0 )
			{
				_size = 1;
			}

            void * mem = nullptr;
			allocator_size_t pi = 0;

			allocator_size_t align_size = memory_buf_align( _size );

			allocator_pool_loop( allocator_pool_alloc )
			{
                allocator_size_t total_size = memory_buf_total( _size );

				mem = detail::stdex_pool_allocator::s_malloc( total_size, _doc );

				if( mem == nullptr )
				{
					return nullptr;
				}

				pi = total_size;
			}

			pool_setup_index( mem, pi );

			void * mem_buff = pool_to_memory( mem );

			return mem_buff;
		}
		//////////////////////////////////////////////////////////////////////////
#define allocator_pool_free(i)\
    if( pi == i )\
			    {\
    allocator_pool(i).free_block(mem_pool);\
			    } else
		//////////////////////////////////////////////////////////////////////////
		static void s_free( void * _mem, const char * _doc )
		{
			if( _mem == nullptr )
			{
				return;
			}

			allocator_size_t pi = memory_to_index( _mem );
			void * mem_pool = memory_to_pool( _mem );

			allocator_pool_loop( allocator_pool_free )
			{
				void * global_mem_pool = memory_to_pool( _mem );
				detail::stdex_pool_allocator::s_free( global_mem_pool, _doc );
			}
		}
		//////////////////////////////////////////////////////////////////////////
		static void * s_realloc( void * _mem, size_t _size, const char * _doc )
		{
			if( _mem == nullptr )
			{
				void * mem = s_malloc( _size, _doc );

				return mem;
			}

			if( _size == 0 )
			{
				s_free( _mem, _doc );

				return nullptr;
			}

			allocator_size_t pi = memory_to_index( _mem );

			if( pi >= si_count )
			{
				void * mem_pool = memory_to_pool( _mem );
				allocator_size_t total_size = memory_buf_total( _size );
				void * realloc_mem = detail::stdex_pool_allocator::s_realloc( mem_pool, total_size, _doc );

				if( realloc_mem == nullptr )
				{
					return nullptr;
				}

				pool_setup_index( realloc_mem, total_size );

				void * realloc_mem_buff = pool_to_memory( realloc_mem );

				return realloc_mem_buff;
			}

            allocator_size_t pool_size = s[pi];

			if( pool_size >= _size )
			{
				return _mem;
			}

			void * new_mem = s_malloc( _size, _doc );

			stdex::memorycopy( new_mem, 0, _mem, pool_size );

			s_free( _mem, _doc );

			return new_mem;
		}
		//////////////////////////////////////////////////////////////////////////
#define allocator_pool_clear(i) \
					{ \
	allocator_pool(i).clear(); \
					}
		//////////////////////////////////////////////////////////////////////////
		static void s_finalize()
		{
			allocator_pool_loop( allocator_pool_clear );
		}
		//////////////////////////////////////////////////////////////////////////
		static void s_thread_lock()
		{
			if( s_thread_lock_func == nullptr )
			{
				return;
			}

			(*s_thread_lock_func)(s_thread_ptr);
		}
		//////////////////////////////////////////////////////////////////////////
		static void s_thread_unlock()
		{
			if( s_thread_unlock_func == nullptr )
			{
				return;
			}

			(*s_thread_unlock_func)(s_thread_ptr);
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize()
		{
			s_initialize = true;
		}
		//////////////////////////////////////////////////////////////////////////
#   ifdef STDEX_ALLOCATOR_DISABLE

		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc( size_t _size, const char * _doc )
		{
            SDTEX_ALLOCATOR_INITIALIZE_CHECK;

            void * mem = detail::stdex_pool_allocator::s_malloc( _size, _doc );

            return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free( void * _mem, const char * _doc )
		{
            SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			detail::stdex_pool_allocator::s_free( _mem, _doc );
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc( size_t _num, size_t _size, const char * _doc )
		{
            SDTEX_ALLOCATOR_INITIALIZE_CHECK;

            size_t full_size = _num * _size;
			
			void * mem = detail::stdex_pool_allocator::s_malloc( full_size, _doc );
			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc( void * _mem, size_t _size, const char * _doc )
		{
            SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			return detail::stdex_pool_allocator::s_realloc( _mem, _size, _doc );
		}
		//////////////////////////////////////////////////////////////////////////
#else
		//////////////////////////////////////////////////////////////////////////
		void * stdex_malloc( size_t _size, const char * _doc )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_malloc( _size, _doc );
			STDEX_ALLOCATOR_UNLOCK();

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_free( void * _mem, const char * _doc )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			STDEX_ALLOCATOR_LOCK();
			stdex::s_free( _mem, _doc );
			STDEX_ALLOCATOR_UNLOCK();
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_calloc( size_t _num, size_t _size, const char * _doc )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			size_t full_size = _num * _size;

			STDEX_ALLOCATOR_LOCK();
			void * mem = stdex::s_malloc( full_size, _doc );
			STDEX_ALLOCATOR_UNLOCK();

			::memset( mem, 0, full_size );

			return mem;
		}
		//////////////////////////////////////////////////////////////////////////
		void * stdex_realloc( void * _mem, size_t _size, const char * _doc )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			STDEX_ALLOCATOR_LOCK();
			void * memory = stdex::s_realloc( _mem, _size, _doc );
			STDEX_ALLOCATOR_UNLOCK();

			return memory;
		}
		//////////////////////////////////////////////////////////////////////////
#endif
		//////////////////////////////////////////////////////////////////////////
#define allocator_pool_bound( i )\
	if( _size < stdex::allocator_pool_size(i) )\
			{\
	bound = stdex::allocator_pool_size(i);\
			} else
		//////////////////////////////////////////////////////////////////////////
        allocator_size_t stdex_allocator_bound( allocator_size_t _size )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			size_t bound = 0;
			allocator_pool_loop( allocator_pool_bound )
			{
				return _size + 4;
			}

			return bound;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize()
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			stdex::s_finalize();

			s_initialize = false;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_initialize_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock )
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			s_thread_ptr = _ptr;
			s_thread_lock_func = _lock;
			s_thread_unlock_func = _unlock;
		}
		//////////////////////////////////////////////////////////////////////////
		void stdex_allocator_finalize_threadsafe()
		{
			SDTEX_ALLOCATOR_INITIALIZE_CHECK;

			s_thread_ptr = nullptr;
			s_thread_lock_func = nullptr;
			s_thread_unlock_func = nullptr;
		}
	}
#ifdef __cplusplus
};
#endif
