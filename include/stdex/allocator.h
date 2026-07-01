#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size );
    void * stdex_realloc( void * _mem, size_t _size );
    void stdex_free( void * _mem );

    void * stdex_malloc_threadsafe( size_t _size );
    void stdex_free_threadsafe( void * _mem );
#ifdef __cplusplus
}
#endif
