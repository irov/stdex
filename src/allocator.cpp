#   include "stdex/allocator.h"
#   include "stdex/pool.h"
#	include "stdex/memorycopy.h"

#   include <malloc.h>
#   include <memory.h>


#   ifndef stdex_allocator_malloc
#   define stdex_allocator_malloc (::malloc)
#   endif

#   ifndef stdex_allocator_realloc
#   define stdex_allocator_realloc (::realloc)
#   endif

#   ifndef stdex_allocator_free
#   define stdex_allocator_free (::free)
#   endif

namespace stdex
{
    //////////////////////////////////////////////////////////////////////////    
    inline static unsigned int mem_get_le32( const unsigned char * _mem )
    {
        unsigned int val;
        const unsigned char * bytes = static_cast<const unsigned char *>(_mem);

        val = bytes[0];
        val |= bytes[1] << 8;
        val |= bytes[2] << 16;
        val |= bytes[3] << 24;

        return val;
    }
    //////////////////////////////////////////////////////////////////////////
    inline static void mem_set_le32( void * _mem, unsigned int _value )
    {
        unsigned char * bytes = static_cast<unsigned char *>(_mem);

        bytes[0] = _value & 0xFF;
        bytes[1] = (_value >> 8) & 0xFF;
        bytes[2] = (_value >> 16) & 0xFF;
        bytes[3] = (_value >> 24) & 0xFF;
    }

#   define memory_buf_allign(f) (f / 4 * 4 + (f % 4) * 4 + 4)
#   define pool_setup_from_memory(m, i) mem_set_le32(m, i)
#   define memory_to_pool(m) static_cast<void *>(static_cast<unsigned char *>(m) - sizeof(int))
#   define memory_to_index(m) mem_get_le32( (static_cast<unsigned char *>(m) - sizeof(int)) )
#   define pool_to_memory(m) static_cast<void *>(static_cast<unsigned char *>(m) + sizeof(int))

#   define allocator_pool_def(i, f, c)\
    const static size_t s##i = f;\
    typedef pool<memory_buf_allign(f), c> pool_type_p##i;\
    static pool_type_p##i p##i;

    allocator_pool_def(0, 8, 128);
    allocator_pool_def(1, 21, 128);
    allocator_pool_def(2, 34, 128);
    allocator_pool_def(3, 55, 128);
    allocator_pool_def(4, 89, 64);
    allocator_pool_def(5, 144, 64);
    allocator_pool_def(6, 233, 64);
    allocator_pool_def(7, 377, 64);
    allocator_pool_def(8, 610, 32);
    allocator_pool_def(9, 987, 32);
    allocator_pool_def(10, 1597, 32);
    allocator_pool_def(11, 2584, 32);
    allocator_pool_def(12, 4181, 8);
    allocator_pool_def(13, 6765, 8);
    allocator_pool_def(14, 10946, 8);
    allocator_pool_def(15, 17711, 8);
    allocator_pool_def(16, 28657, 4);
    allocator_pool_def(17, 46368, 4);
    allocator_pool_def(18, 75025, 4);
    allocator_pool_def(19, 121393, 4);

#	ifdef stdex_allocator_threadsafe
	void * g_thread_ptr = nullptr;
	stdex_thread_lock_t g_thread_lock = nullptr;
	stdex_thread_unlock_t g_thread_unlock = nullptr;
#	endif

    //////////////////////////////////////////////////////////////////////////
    const static size_t s[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19};
    //////////////////////////////////////////////////////////////////////////
    const static int si_global = 20;
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
    if( _size < s##i )\
    {\
        mem = p##i.alloc_block();\
        pi = i;\
    } else
    //////////////////////////////////////////////////////////////////////////
    static void * s_malloc( size_t _size )
    {
        if( _size == 0 )
        {
            _size = 1;
        }

        void * mem = 0;
        int pi = 0;

        allocator_pool_loop( allocator_pool_alloc )
        {
            size_t allign_size = memory_buf_allign(_size);
            mem = stdex_allocator_malloc(allign_size);
            pi = si_global;
        }

        pool_setup_from_memory(mem, pi);

        void * mem_buff = pool_to_memory(mem);

        return mem_buff;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_free(i)\
    if( pi == i )\
    {\
    p##i.free_block(mem_pool);\
    } else
    //////////////////////////////////////////////////////////////////////////
    static void s_free( void * _mem )
    {
        if( _mem == nullptr )
        {
            return;
        }

        int pi = memory_to_index(_mem);
        void * mem_pool = memory_to_pool(_mem);

        allocator_pool_loop( allocator_pool_free )
        {
            stdex_allocator_free(mem_pool);
        }
    }
    //////////////////////////////////////////////////////////////////////////
    static void * s_realloc( void * _mem, size_t _size )
    {
        if( _mem == nullptr )
        {
            void * mem = s_malloc( _size );

            return mem;
        }

        int pi = memory_to_index(_mem);

        if( pi == si_global )
        {
            void * mem_pool = memory_to_pool(_mem);

            void * realloc_mem = stdex_allocator_realloc( mem_pool, memory_buf_allign(_size) );
                        
            void * realloc_mem_buff = pool_to_memory(realloc_mem);

            return realloc_mem_buff;
        }

        size_t pool_size = s[pi];

        if( pool_size > _size )
        {
            return _mem;
        }

        void * new_mem = s_malloc( _size );

		stdex::memorycopy( new_mem, _mem, pool_size );
        
        s_free( _mem );
        
        return new_mem;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_info(i) \
    { \
        pool_type_p##i & p = p##i; \
        _info[i].block_size = p.getBlockSize(); \
        _info[i].chunk_count = p.getChunkCount(); \
        _info[i].block_count = p.getBlockCount(); \
        _info[i].block_total = p.getBlockTotal(); \
		++count; \
    }
    //////////////////////////////////////////////////////////////////////////
    static size_t s_memoryinfo( stdex_memory_info_t * _info, size_t _count )
    {
		(void)_count;

		size_t count = 0;
        allocator_pool_loop( allocator_pool_info );

		return count;			 
    }
	//////////////////////////////////////////////////////////////////////////
	static size_t s_memorytotal()
	{
		stdex_memory_info_t mi[25];
		size_t count = s_memoryinfo( mi, 25 );

		size_t total_now = 0;
		
		for( size_t i = 0; i != count; ++i )
		{
			total_now += mi[i].block_size * mi[i].block_count;
		}

		return total_now;
	}
	//////////////////////////////////////////////////////////////////////////
	inline static void s_thread_lock()
	{
#	ifdef stdex_allocator_threadsafe
		if( stdex::g_thread_lock == nullptr )
		{
			return;
		}
		
		(*stdex::g_thread_lock)( g_thread_ptr );
#	endif
	}
	//////////////////////////////////////////////////////////////////////////
	inline static void s_thread_unlock()
	{
#	ifdef stdex_allocator_threadsafe
		if( stdex::g_thread_unlock == nullptr )
		{
			return;
		}

		(*stdex::g_thread_unlock)( g_thread_ptr );
#	endif
	}
}

#ifdef __cplusplus
extern "C" {
#endif
#	ifdef stdex_allocator_threadsafe
	//////////////////////////////////////////////////////////////////////////
	void stdex_threadsafe( void * _ptr, stdex_thread_lock_t _lock, stdex_thread_unlock_t _unlock )
	{
		stdex::g_thread_ptr = _ptr;
		stdex::g_thread_lock = _lock;
		stdex::g_thread_unlock = _unlock;
	}
#	endif

#   ifdef stdex_allocator_disable
    //////////////////////////////////////////////////////////////////////////
    void * stdex_malloc( size_t _size )
    {
        return stdex_allocator_malloc( _size );
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_free( void * _mem )
    {
        stdex_allocator_free( _mem );
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
        void * mem = stdex_allocator_malloc( full_size );
        ::memset( mem, 0, full_size );

        return mem;
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_realloc( void * _mem, size_t _size )
    {
        return stdex_allocator_realloc( _mem, _size );
    }
    //////////////////////////////////////////////////////////////////////////
#   else
    //////////////////////////////////////////////////////////////////////////
    void * stdex_malloc( size_t _size )
    {
		stdex::s_thread_lock();
        void * memory = stdex::s_malloc( _size );
		stdex::s_thread_unlock();

		return memory;
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_free( void * _mem )
    {
		stdex::s_thread_lock();
        stdex::s_free( _mem );
		stdex::s_thread_unlock();
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
        void * mem = stdex::s_malloc( full_size );
        ::memset( mem, 0, full_size );

        return mem;
    }
    //////////////////////////////////////////////////////////////////////////
    void * stdex_realloc( void * _mem, size_t _size )
    {
		stdex::s_thread_lock();
        void * memory = stdex::s_realloc( _mem, _size );
		stdex::s_thread_unlock();

		return memory;
    }
    //////////////////////////////////////////////////////////////////////////
#   endif
    //////////////////////////////////////////////////////////////////////////
    size_t stdex_memoryinfo( stdex_memory_info_t * _info, size_t _count )
    {
        return stdex::s_memoryinfo( _info, _count );
    }
	//////////////////////////////////////////////////////////////////////////
	size_t stdex_memorytotal()
	{
		return stdex::s_memorytotal();
	}
#ifdef __cplusplus
};
#endif
