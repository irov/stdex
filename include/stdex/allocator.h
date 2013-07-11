#   pragma once

#   include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size );
    void stdex_free( void * _mem );
    void * stdex_calloc( size_t _num, size_t _size );
    void * stdex_realloc( void * _mem, size_t _size );
#ifdef __cplusplus
};
#endif
