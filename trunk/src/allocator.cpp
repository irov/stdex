#   include "stdex/allocator.h"
#   include "stdex/pool.h"

#   include <malloc.h>
#   include <memory.h>

namespace stdex
{
#   define allocator_pool_def(i, f, c)\
    const static size_t s##i = f;\
    typedef pool<f, c> pool_type_p##i;\
    pool_type_p##i p##i;

    allocator_pool_def(0, 8, 32);
    allocator_pool_def(1, 21, 32);
    allocator_pool_def(2, 34, 32);
    allocator_pool_def(3, 55, 32);
    allocator_pool_def(4, 89, 32);
    allocator_pool_def(5, 144, 32);
    allocator_pool_def(6, 233, 32);
    allocator_pool_def(7, 377, 32);
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
    allocator_pool_def(19, 121393, 2);
    //////////////////////////////////////////////////////////////////////////
    const static size_t s[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19};
    //////////////////////////////////////////////////////////////////////////
    const static size_t si_global = 20;
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_loop( function )\
        function(0) else \
        function(1) else \
        function(2) else \
        function(3) else \
        function(4) else \
        function(5) else \
        function(6) else \
        function(7) else \
        function(8) else \
        function(9) else \
        function(10) else \
        function(11) else \
        function(12) else \
        function(13) else \
        function(14) else \
        function(15) else \
        function(16) else \
        function(17) else \
        function(18) else \
        function(19) else

    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_alloc( i )\
    if( _size < s##i )\
    {\
        mem = p##i.alloc();\
        pi = i;\
    }
    //////////////////////////////////////////////////////////////////////////
    static void * malloc( size_t _size )
    {
        if( _size == 0 )
        {
            _size = 1;
        }

        void * mem = 0;
        unsigned char pi = 0;

        allocator_pool_loop( allocator_pool_alloc )
        {
            mem = ::malloc(_size + 1);
            pi = si_global;
        }

        unsigned char * mem_uc = static_cast<unsigned char *>(mem);
        mem_uc[0] = pi;

        void * mem_buff = static_cast<void *>(mem_uc + 1);

        return mem_buff;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_free(i)\
    if( pi == i )\
    {\
    p##i.free(mem_pool);\
    }
    //////////////////////////////////////////////////////////////////////////
    static void free( void * _mem )
    {
        if( _mem == nullptr )
        {
            return;
        }

        unsigned char * mem_uc = static_cast<unsigned char *>(_mem);
        unsigned char * mem_i = mem_uc - 1;

        unsigned char pi = mem_i[0];

        void * mem_pool = static_cast<void *>(mem_i);

        allocator_pool_loop( allocator_pool_free )
        {
            ::free(mem_pool);
        }
    }
    //////////////////////////////////////////////////////////////////////////
    static void * calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
        void * mem = malloc( full_size );
        ::memset( mem, 0, full_size );

        return mem;
    }    
    //////////////////////////////////////////////////////////////////////////
    static void * realloc( void * _mem, size_t _size )
    {
        if( _mem == nullptr )
        {
            void * mem = malloc( _size );

            return mem;
        }

        unsigned char * mem_uc = static_cast<unsigned char *>(_mem);
        unsigned char * mem_i = mem_uc - 1;

        unsigned char pi = mem_i[0];

        if( pi == si_global )
        {
            void * mem_pool = static_cast<void *>(mem_i);

            void * realloc_mem = ::realloc( mem_pool, _size + 1 );

            unsigned char * mem_uc = static_cast<unsigned char *>(realloc_mem);

            void * mem_buff = static_cast<void *>(mem_uc + 1);

            return mem_buff;
        }

        size_t pool_size = s[pi];

        if( pool_size > _size )
        {
            return _mem;
        }

        void * new_mem = malloc( _size );

        ::memcpy( new_mem, _mem, pool_size );
        
        free( _mem );
        
        return new_mem;
    }
}

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size )
    {
        return stdex::malloc( _size );
    }

    void stdex_free( void * _mem )
    {
        stdex::free( _mem );
    }

    void * stdex_calloc( size_t _num, size_t _size )
    {
        return stdex::calloc( _num, _size );
    }

    void * stdex_realloc( void * _mem, size_t _size )
    {
        return stdex::realloc( _mem, _size );
    }
#ifdef __cplusplus
};
#endif
