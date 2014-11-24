#   include "stdex/allocator.h"
#   include "stdex/pool.h"
#	include "stdex/memorycopy.h"

#   include <stdint.h>
#   include <memory.h>

//#	define STDEX_ALLOCATOR_DISABLE

#   ifndef STDEX_ALLOCATOR_INCLUDE
#	define STDEX_ALLOCATOR_INCLUDE <malloc.h>
#	endif

#   ifndef STDEX_ALLOCATOR_MALLOC
#   define STDEX_ALLOCATOR_MALLOC (::malloc)
#   endif

#   ifndef STDEX_ALLOCATOR_REALLOC
#   define STDEX_ALLOCATOR_REALLOC (::realloc)
#   endif

#   ifndef STDEX_ALLOCATOR_FREE
#   define STDEX_ALLOCATOR_FREE (::free)
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
	void * g_thread_ptr = nullptr;
	stdex_allocator_thread_lock_t g_thread_lock = nullptr;
	stdex_allocator_thread_unlock_t g_thread_unlock = nullptr;
	//////////////////////////////////////////////////////////////////////////
	inline static void s_thread_lock()
	{
		if( stdex::g_thread_lock == nullptr )
		{
			return;
		}

		(*stdex::g_thread_lock)( g_thread_ptr );
	}
	//////////////////////////////////////////////////////////////////////////
	inline static void s_thread_unlock()
	{
		if( stdex::g_thread_unlock == nullptr )
		{
			return;
		}

		(*stdex::g_thread_unlock)( g_thread_ptr );
	}
	//////////////////////////////////////////////////////////////////////////    
	inline static uint32_t mem_get_value( const void * _mem )
	{
		const uint32_t * bytes = static_cast<const uint32_t *>(_mem);

		uint32_t value = bytes[0];
		//value |= bytes[1] << 8;
		//value |= bytes[2] << 16;
		//value |= bytes[3] << 24;

		return value;
	}
	//////////////////////////////////////////////////////////////////////////
	inline static void mem_set_value( void * _mem, uint32_t _value )
	{
		uint32_t * bytes = static_cast<uint32_t *>(_mem);

		bytes[0] = _value;

		//bytes[0] = _value & 0xFF;
		//bytes[1] = (_value >> 8) & 0xFF;
		//bytes[2] = (_value >> 16) & 0xFF;
		//bytes[3] = (_value >> 24) & 0xFF;
	}
	//////////////////////////////////////////////////////////////////////////
#   define memory_buf_allign(f) (((f + 3) / 4) * 4)
#   define memory_buf_total(f) (memory_buf_allign(f) + sizeof(uint32_t))

#   define pool_to_memory(m) static_cast<void *>(static_cast<unsigned char *>(m) + sizeof(uint32_t))

#   define pool_setup_index(m, i) mem_set_value(m, i)

#   define memory_ptr(m) (static_cast<unsigned char *>(m) - sizeof(uint32_t))
#   define memory_to_pool(m) static_cast<void *>(memory_ptr(m))
#   define memory_to_index(m) mem_get_value( memory_ptr(m) )


#	define allocator_pool(i) s_p##i
#	define allocator_pool_size(i) s_s##i

#   define allocator_pool_def(i, f, c)\
    const static size_t allocator_pool_size(i) = memory_buf_allign(f);\
    typedef pool<memory_buf_total(f), c> t_pool_type_p##i;\
    static t_pool_type_p##i allocator_pool(i);

	allocator_pool_def(0, 4, 128);
	allocator_pool_def(1, 8, 2048);
	allocator_pool_def(2, 13, 1024);
	allocator_pool_def(3, 17, 1024);
	allocator_pool_def(4, 21, 512);
	allocator_pool_def(5, 27, 512);
	allocator_pool_def(6, 34, 256);
	allocator_pool_def(7, 44, 256);
	allocator_pool_def(8, 55, 256);
	allocator_pool_def(9, 72, 256);
	allocator_pool_def(10, 89, 256);
	allocator_pool_def(11, 116, 256);
	allocator_pool_def(12, 144, 128);
	allocator_pool_def(13, 188, 128);
	allocator_pool_def(14, 233, 128);
	allocator_pool_def(15, 305, 128);
	allocator_pool_def(16, 377, 128);
	allocator_pool_def(17, 493, 128);
	allocator_pool_def(18, 610, 64);
	allocator_pool_def(19, 798, 64);
	//////////////////////////////////////////////////////////////////////////
	const static uint32_t si_count = 20;
	//////////////////////////////////////////////////////////////////////////
	static size_t s_global_memory_use = 0U;
    //////////////////////////////////////////////////////////////////////////
	const static size_t s[] = 
	{ allocator_pool_size(0)
	, allocator_pool_size(1)
	, allocator_pool_size(2)
	, allocator_pool_size(3)
	, allocator_pool_size(4)
	, allocator_pool_size(5)
	, allocator_pool_size(6)
	, allocator_pool_size(7)
	, allocator_pool_size(8)
	, allocator_pool_size(9)
	, allocator_pool_size(10)
	, allocator_pool_size(11)
	, allocator_pool_size(12)
	, allocator_pool_size(13)
	, allocator_pool_size(14)
	, allocator_pool_size(15)
	, allocator_pool_size(16)
	, allocator_pool_size(17)
	, allocator_pool_size(18)
	, allocator_pool_size(19) 
	};
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

    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_alloc( i )\
    if( align_size <= allocator_pool_size(i) )\
    {\
        mem = allocator_pool(i).alloc_block();\
        pi = i;\
    } else
    //////////////////////////////////////////////////////////////////////////
    static void * s_malloc( size_t _size )
    {
        if( _size == 0 )
        {
            _size = 1;
        }

        void * mem = nullptr;
        uint32_t pi = 0;

		size_t align_size = memory_buf_allign(_size);

        allocator_pool_loop( allocator_pool_alloc )
        {
			size_t total_size = memory_buf_total(_size);
			mem = STDEX_ALLOCATOR_MALLOC(total_size);

			if( mem == nullptr )
			{
				return nullptr;
			}

			s_global_memory_use += total_size;

			pi = (uint32_t)total_size;
        }

        pool_setup_index(mem, pi);

        void * mem_buff = pool_to_memory(mem);

        return mem_buff;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_free(i)\
    if( pi == i )\
    {\
    allocator_pool(i).free_block(mem_pool);\
    } else
    //////////////////////////////////////////////////////////////////////////
    static void s_free( void * _mem )
    {
        if( _mem == nullptr )
        {
            return;
        }

        uint32_t pi = memory_to_index(_mem);
        void * mem_pool = memory_to_pool(_mem);

        allocator_pool_loop( allocator_pool_free )
        {			
			size_t global_mem_size = (size_t)pi;
			s_global_memory_use -= global_mem_size;

			void * global_mem_pool = memory_to_pool(_mem);			
            STDEX_ALLOCATOR_FREE(global_mem_pool);
        }
    }
    //////////////////////////////////////////////////////////////////////////
    static void * s_realloc( void * _mem, size_t _size )
    {
        if( _mem == nullptr )
        {
			STDEX_ALLOCATOR_LOCK();
            void * mem = s_malloc( _size );
			STDEX_ALLOCATOR_UNLOCK();

            return mem;
        }

		if( _size == 0 )
		{
			STDEX_ALLOCATOR_LOCK();
			s_free( _mem );
			STDEX_ALLOCATOR_UNLOCK();

			return nullptr;
		}

        uint32_t pi = memory_to_index(_mem);

        if( pi >= si_count )
        {
			size_t mem_size = (size_t)pi;

            void * mem_pool = memory_to_pool(_mem);
			size_t total_size = memory_buf_total(_size);
            void * realloc_mem = STDEX_ALLOCATOR_REALLOC( mem_pool, total_size );

			if( realloc_mem == nullptr )
			{
				return nullptr;
			}
			
			pool_setup_index(realloc_mem, (uint32_t)total_size);

            void * realloc_mem_buff = pool_to_memory(realloc_mem);

			STDEX_ALLOCATOR_LOCK();
			s_global_memory_use += total_size - mem_size;
			STDEX_ALLOCATOR_UNLOCK();

            return realloc_mem_buff;
        }

        size_t pool_size = s[pi];

        if( pool_size >= _size )
        {
            return _mem;
        }

		STDEX_ALLOCATOR_LOCK();
        void * new_mem = s_malloc( _size );

		stdex::memorycopy( new_mem, _mem, pool_size );
        
        s_free( _mem );
		STDEX_ALLOCATOR_UNLOCK();
        
        return new_mem;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_info(i) \
    { \
        _info[i].block_size = allocator_pool(i).getBlockSize(); \
        _info[i].chunk_count = allocator_pool(i).getChunkCount(); \
        _info[i].block_count = allocator_pool(i).getBlockCount(); \
        _info[i].block_total = allocator_pool(i).getBlockTotal(); \
		++count; \
    }
    //////////////////////////////////////////////////////////////////////////
    static uint32_t s_memoryinfo( stdex_memory_info_t * _info, uint32_t _count )
    {
		if( si_count > _count )
		{
			return 0;
		}

		uint32_t count = 0;
        allocator_pool_loop( allocator_pool_info );

		return count;
    }
	//////////////////////////////////////////////////////////////////////////
	static size_t s_memoryuse()
	{
		stdex_memory_info_t mi[25];
		uint32_t count = s_memoryinfo( mi, 25 );

		size_t total_now = 0;
		
		for( uint32_t i = 0; i != count; ++i )
		{
			total_now += mi[i].block_size * mi[i].block_count;
		}

		total_now += s_global_memory_use;

		return total_now;
	}
}

#ifdef __cplusplus
extern "C" {
#endif
	//////////////////////////////////////////////////////////////////////////
	void stdex_allocator_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock )
	{
		stdex::g_thread_ptr = _ptr;
		stdex::g_thread_lock = _lock;
		stdex::g_thread_unlock = _unlock;
	}

#   ifdef STDEX_ALLOCATOR_DISABLE
    //////////////////////////////////////////////////////////////////////////
    void * stdex_malloc( size_t _size )
    {
        return STDEX_ALLOCATOR_MALLOC( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_free( void * _mem )
    {
        STDEX_ALLOCATOR_FREE( _mem );
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
        void * mem = STDEX_ALLOCATOR_MALLOC( full_size );
        ::memset( mem, 0, full_size );

        return mem;
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_realloc( void * _mem, size_t _size )
    {
        return STDEX_ALLOCATOR_REALLOC( _mem, _size );
    }
    //////////////////////////////////////////////////////////////////////////
#   else
    //////////////////////////////////////////////////////////////////////////
    void * stdex_malloc( size_t _size )
    {
		STDEX_ALLOCATOR_LOCK();
        void * memory = stdex::s_malloc( _size );
		STDEX_ALLOCATOR_UNLOCK();

		return memory;
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_free( void * _mem )
    {
		STDEX_ALLOCATOR_LOCK();
        stdex::s_free( _mem );
		STDEX_ALLOCATOR_UNLOCK();
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
		
		STDEX_ALLOCATOR_LOCK();
        void * mem = stdex::s_malloc( full_size );
		STDEX_ALLOCATOR_UNLOCK();

        ::memset( mem, 0, full_size );
		
        return mem;
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_realloc( void * _mem, size_t _size )
    {		
        void * memory = stdex::s_realloc( _mem, _size );
		
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
		uint32_t bound = 0;
		allocator_pool_loop( allocator_pool_bound )
		{
			return _size + 4;
		}

		return bound;
	}
	//////////////////////////////////////////////////////////////////////////
	size_t stdex_allocator_globalmemoryuse()
	{
		size_t memory = stdex::s_global_memory_use;

		return memory;
	}
	//////////////////////////////////////////////////////////////////////////
	size_t stdex_allocator_memoryuse()
	{
		size_t memory = stdex::s_memoryuse();

		return memory;
	}
    //////////////////////////////////////////////////////////////////////////
    uint32_t stdex_allocator_memoryinfo( stdex_memory_info_t * _info, uint32_t _count )
    {
        return stdex::s_memoryinfo( _info, _count );
    }
#ifdef __cplusplus
};
#endif
