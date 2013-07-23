#   include "stdex/allocator.h"
#   include "stdex/pool.h"

#   include <malloc.h>
#   include <memory.h>

#   include <stdio.h>

namespace stdex
{
#   define allocator_pool_def(i, f, c)\
    const static size_t s##i = f;\
    typedef pool<f, c> pool_type_p##i;\
    pool_type_p##i p##i;

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
    const static size_t s[] = {s0, s1, s2, s3, s4, s5, s6, s7, s8, s9, s10, s11, s12, s13, s14, s15, s16, s17, s18, s19};
    //////////////////////////////////////////////////////////////////////////
    const static size_t si_global = 20;
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
        mem = p##i.alloc();\
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
    } else
    //////////////////////////////////////////////////////////////////////////
    static void s_free( void * _mem )
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
    static void * s_calloc( size_t _num, size_t _size )
    {
        size_t full_size = _num * _size;
        void * mem = s_malloc( full_size );
        ::memset( mem, 0, full_size );

        return mem;
    }    
    //////////////////////////////////////////////////////////////////////////
    static void * s_realloc( void * _mem, size_t _size )
    {
        if( _mem == nullptr )
        {
            void * mem = s_malloc( _size );

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

        void * new_mem = s_malloc( _size );

        ::memcpy( new_mem, _mem, pool_size );
        
        s_free( _mem );
        
        return new_mem;
    }
    //////////////////////////////////////////////////////////////////////////
#   define allocator_pool_info(i) \
    { \
        pool_type_p##i & p = p##i; \
        size_t bs = p.getBlockSize(); \
        size_t cc = p.getChunkCount(); \
        size_t bc = p.getBlockCount(); \
        size_t bt = p.getBlockTotal(); \
        total_now += bs * bc; \
        total_max += bs * bt * cc; \
        printf("block %d:%d %d alloc %d:%d\n", bs, cc, bc, bs * bc, bs * bt * cc ); \
    }
    //////////////////////////////////////////////////////////////////////////
    static void s_memoryinfo()
    {
        printf("-------------------------------------\n");

        size_t total_now = 0;
        size_t total_max = 0;
        allocator_pool_loop( allocator_pool_info );
        
        float total_now_mb = float(total_now / (1024.f * 1024.f));
        float total_max_mb = float(total_max / (1024.f * 1024.f));
        printf("-------------------------------------\n");
        printf("total %.3f:%.3f\n", total_now_mb, total_max_mb);
    }
}

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size )
    {
        return stdex::s_malloc( _size );
    }

    void stdex_free( void * _mem )
    {
        stdex::s_free( _mem );
    }

    void * stdex_calloc( size_t _num, size_t _size )
    {
        return stdex::s_calloc( _num, _size );
    }

    void * stdex_realloc( void * _mem, size_t _size )
    {
        return stdex::s_realloc( _mem, _size );
    }

    void stdex_memoryinfo()
    {
        return stdex::s_memoryinfo();
    }
#ifdef __cplusplus
};
#endif
