#   pragma once

#   include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size );
    void stdex_free( void * _mem );
    void * stdex_calloc( size_t _num, size_t _size );
    void * stdex_realloc( void * _mem, size_t _size );

	struct stdex_memory_info_t
	{
		size_t block_size;
		size_t chunk_count;
		size_t block_count;
		size_t block_total;
	};

    size_t stdex_memoryinfo( struct stdex_memory_info_t * _info, size_t _count );
	size_t stdex_memorytotal();
#ifdef __cplusplus
};
#endif
