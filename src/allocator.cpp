#   include "stdex/allocator.h"
#   include "stdex/pool.h"
#	include "stdex/memorycopy.h"

#   include <malloc.h>
#   include <stdint.h>
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
	inline static unsigned int mem_get_value( const void * _mem )
	{
		const unsigned char * bytes = static_cast<const unsigned char *>(_mem);

		unsigned int value = bytes[0];
		value |= bytes[1] << 8;
		value |= bytes[2] << 16;
		value |= bytes[3] << 24;

		return value;
	}
	//////////////////////////////////////////////////////////////////////////
	inline static void mem_set_value( void * _mem, unsigned int _value, size_t _offset )
	{
		unsigned char * bytes = static_cast<unsigned char *>(_mem) + sizeof(int) * _offset;

		bytes[0] = _value & 0xFF;
		bytes[1] = (_value >> 8) & 0xFF;
		bytes[2] = (_value >> 16) & 0xFF;
		bytes[3] = (_value >> 24) & 0xFF;
	}
	//////////////////////////////////////////////////////////////////////////
#   define memory_buf_allign(f) (((f + 3) / 4) * 4 + 4)

#   define pool_to_memory(m) static_cast<void *>(static_cast<unsigned char *>(m) + sizeof(int))

#   define pool_setup_index(m, i) mem_set_value(m, i, 0)

#   define memory_ptr(m) (static_cast<unsigned char *>(m) - sizeof(int))
#   define memory_to_pool(m) static_cast<void *>(memory_ptr(m))
#   define memory_to_index(m) mem_get_value( memory_ptr(m) )

#   define global_memory_buf_allign(f) (((f + 3) / 4) * 4 + 4 + 4)

#   define global_pool_to_memory(m) static_cast<void *>(static_cast<unsigned char *>(m) + sizeof(int) + sizeof(int))

#   define global_pool_setup_index(m, i) mem_set_value(m, i, 1)
#   define global_pool_setup_size(m, i) mem_set_value(m, i, 0)

#   define global_memory_ptr(m) (static_cast<unsigned char *>(m) - sizeof(int) - sizeof(int))
#   define global_memory_to_pool(m) static_cast<void *>(global_memory_ptr(m))
#   define global_memory_to_size(m) mem_get_value( global_memory_ptr(m) )



#	define allocator_pool(i) s_p##i
#	define allocator_pool_size(i) s_s##i

#   define allocator_pool_def(i, f, c)\
    const static size_t allocator_pool_size(i) = f;\
    typedef pool<memory_buf_allign(f), c> t_pool_type_p##i;\
    static t_pool_type_p##i allocator_pool(i);

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
	//////////////////////////////////////////////////////////////////////////
	const static int si_count = 20;
	//////////////////////////////////////////////////////////////////////////
	const static int si_global = 20;
	//////////////////////////////////////////////////////////////////////////
	static unsigned int s_global_memory_use = 0U;

#	ifdef stdex_allocator_threadsafe
	void * g_thread_ptr = nullptr;
	stdex_thread_lock_t g_thread_lock = nullptr;
	stdex_thread_unlock_t g_thread_unlock = nullptr;
#	endif

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
    if( _size < allocator_pool_size(i) )\
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
        unsigned int pi = 0;

        allocator_pool_loop( allocator_pool_alloc )
        {
			unsigned int allign_size = global_memory_buf_allign(_size);
			mem = stdex_allocator_malloc(allign_size);
		
			global_pool_setup_size(mem, allign_size);
			global_pool_setup_index(mem, si_global);

			s_global_memory_use += allign_size;
			
			void * mem_buff = global_pool_to_memory(mem);

			return mem_buff;
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

        unsigned int pi = memory_to_index(_mem);
        void * mem_pool = memory_to_pool(_mem);

        allocator_pool_loop( allocator_pool_free )
        {			
			unsigned int global_mem_size = global_memory_to_size(_mem);
			s_global_memory_use -= global_mem_size;

			void * global_mem_pool = global_memory_to_pool(_mem);			
            stdex_allocator_free(global_mem_pool);		
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

        unsigned int pi = memory_to_index(_mem);

        if( pi == si_global )
        {
			size_t mem_size = global_memory_to_size(_mem);
			s_global_memory_use -= mem_size;

            void * mem_pool = global_memory_to_pool(_mem);
			size_t allign_size = global_memory_buf_allign(_size);
            void * realloc_mem = stdex_allocator_realloc( mem_pool, allign_size );
			
			s_global_memory_use += allign_size;
			global_pool_setup_size(realloc_mem, allign_size);

            void * realloc_mem_buff = global_pool_to_memory(realloc_mem);

            return realloc_mem_buff;
        }

        size_t pool_size = s[pi];

        if( pool_size >= _size )
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
        _info[i].block_size = allocator_pool(i).getBlockSize(); \
        _info[i].chunk_count = allocator_pool(i).getChunkCount(); \
        _info[i].block_count = allocator_pool(i).getBlockCount(); \
        _info[i].block_total = allocator_pool(i).getBlockTotal(); \
		++count; \
    }
    //////////////////////////////////////////////////////////////////////////
    static size_t s_memoryinfo( stdex_memory_info_t * _info, size_t _count )
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
	static size_t s_memorytotal()
	{
		stdex_memory_info_t mi[25];
		size_t count = s_memoryinfo( mi, 25 );

		size_t total_now = 0;
		
		for( size_t i = 0; i != count; ++i )
		{
			total_now += mi[i].block_size * mi[i].block_count;
		}

		total_now += s_global_memory_use;

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
	size_t stdex_globalmemoryuse()
	{
		return stdex::s_global_memory_use;
	}
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
