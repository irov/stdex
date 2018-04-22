#pragma once

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif
	void stdex_allocator_initialize();
	void stdex_allocator_finalize();

	void * stdex_malloc( size_t _size, const char * _doc );
    void stdex_free( void * _mem, const char * _doc );
    void * stdex_calloc( size_t _num, size_t _size, const char * _doc );
    void * stdex_realloc( void * _mem, size_t _size, const char * _doc );
	
	typedef void( *stdex_allocator_thread_lock_t )(void * _ptr);
	typedef void( *stdex_allocator_thread_unlock_t )(void * _ptr);

	void stdex_allocator_initialize_threadsafe( void * _ptr, stdex_allocator_thread_lock_t _lock, stdex_allocator_thread_unlock_t _unlock );
	void stdex_allocator_finalize_threadsafe();    
#ifdef __cplusplus
};
#endif
