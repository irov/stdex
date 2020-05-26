#pragma once

#include <cstddef>
#include <cstdint>

//#define STDEX_ALLOCATOR_REPORT_ENABLE

#ifndef STDEX_ALLOCATOR_REPORT_MAX_NAME
#   define STDEX_ALLOCATOR_REPORT_MAX_NAME 128
#endif

#ifndef STDEX_ALLOCATOR_REPORT_MAX_COUNT
#   define STDEX_ALLOCATOR_REPORT_MAX_COUNT 1024
#endif

#ifdef __cplusplus
extern "C" {
#endif

    struct stdex_memory_report_t
    {
        char name[STDEX_ALLOCATOR_REPORT_MAX_NAME];
        uint32_t count;
    };

    void stdex_allocator_report_new( uint32_t _size, const char * _name );
    void stdex_allocator_report_free( void * _ptr, const char * _name );
    void stdex_allocator_report_free_n( uint32_t _size, const char * _name );
    void stdex_allocator_report_realoc( void * _ptr, uint32_t _size, const char * _name );

    uint32_t stdex_allocator_report_count();
    struct stdex_memory_report_t * stdex_allocator_report_info( uint32_t _index );
    uint32_t stdex_get_allocator_report_count( const char * _name );

#ifdef __cplusplus
};
#endif
