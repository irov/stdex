#   pragma once

#   include <stddef.h>
#   include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
    void * stdex_malloc( size_t _size );
    void stdex_free( void * _mem );
    void * stdex_calloc( size_t _num, size_t _size );
    void * stdex_realloc( void * _mem, size_t _size );

	typedef void (*stdex_allocator_thread_lock_t)( void * _ptr );
	typedef void (*stdex_allocator_thread_unlock_t)( void * _ptr );

	void stdex_allocator_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock );

	struct stdex_memory_info_t
	{
		size_t block_size;
		size_t chunk_count;
		size_t block_count;
		size_t block_total;
	};

    size_t stdex_allocator_globalmemoryuse();
	size_t stdex_allocator_memoryuse();

	uint32_t stdex_allocator_memoryinfo( struct stdex_memory_info_t * _info, uint32_t _count );
	
#ifdef __cplusplus
};
#endif
