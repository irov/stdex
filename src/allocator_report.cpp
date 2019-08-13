#include "stdex/allocator_report.h"

#include <stdlib.h>
#include <string.h>

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
#include <malloc.h>
#endif

#include <algorithm>

#ifdef __cplusplus
extern "C" {
#endif
    //////////////////////////////////////////////////////////////////////////
    stdex_memory_report_t * mm = nullptr;
#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
    //////////////////////////////////////////////////////////////////////////
    static void stdex_allocator_report( uint32_t _incref, uint32_t _decref, const char * _name )
    {
        (void)_incref;
        (void)_decref;
        (void)_name;

        if( mm == nullptr )
        {
            mm = (stdex_memory_report_t *)malloc( sizeof( stdex_memory_report_t ) * STDEX_ALLOCATOR_REPORT_MAX_COUNT );
            memset( mm, 0, sizeof( stdex_memory_report_t ) * STDEX_ALLOCATOR_REPORT_MAX_COUNT );
        }

        for( uint32_t i = 0; i != STDEX_ALLOCATOR_REPORT_MAX_COUNT; ++i )
        {
            stdex_memory_report_t * m = mm + i;

            if( strlen( m->name ) == 0 )
            {
                strncpy( m->name, _name, STDEX_ALLOCATOR_REPORT_MAX_NAME );
            }
            else if( strcmp( m->name, _name ) != 0 )
            {
                continue;
            }

            m->count += _incref;
            m->count -= _decref;

            break;
        }

        //std::sort( mm, mm + STDEX_ALLOCATOR_REPORT_MAX_COUNT
        //    , []( const stdex_memory_report_t & l, const stdex_memory_report_t & r ) { return l.count > r.count; }
        //);
    }
#endif
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_new( uint32_t _size, const char * _name )
    {
        (void)_size;
        (void)_name;

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
        stdex_allocator_report( _size, 0U, _name );
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_free( void * _ptr, const char * _name )
    {
        (void)_ptr;
        (void)_name;

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
        if( _ptr == 0 )
        {
            return;
        }

        size_t ptr_size = _msize( _ptr );

        stdex_allocator_report( 0U, (uint32_t)ptr_size, _name );
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_free_n( uint32_t _size, const char * _name )
    {
        (void)_size;
        (void)_name;

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
        stdex_allocator_report( 0U, _size, _name );
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_realoc( void * _ptr, uint32_t _size, const char * _name )
    {
        (void)_ptr;
        (void)_size;
        (void)_name;

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
        if( _ptr == 0 )
        {
            stdex_allocator_report( _size, 0U, _name );
        }
        else
        {
            size_t ptr_size = _msize( _ptr );

            stdex_allocator_report( _size, (uint32_t)ptr_size, _name );
        }
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t stdex_allocator_report_count()
    {
        if( mm == nullptr )
        {
            return 0;
        }

        uint32_t u = 0;

        for( uint32_t i = 0; i != STDEX_ALLOCATOR_REPORT_MAX_COUNT; ++i )
        {
            stdex_memory_report_t * m = mm + i;

            if( m->count == 0 )
            {
                break;
            }

            ++u;
        }

        return u;
    }
    //////////////////////////////////////////////////////////////////////////
    struct stdex_memory_report_t * stdex_allocator_report_info( uint32_t _index )
    {
        return mm + _index;
    }
    //////////////////////////////////////////////////////////////////////////
    uint32_t stdex_get_allocator_report_count( const char * _name )
    {
        if( mm == nullptr )
        {
            return 0L;
        }

        for( uint32_t i = 0; i != STDEX_ALLOCATOR_REPORT_MAX_COUNT; ++i )
        {
            stdex_memory_report_t * m = mm + i;

            if( strcmp( m->name, _name ) == 0 )
            {
                return m->count;
            }
        }

        return 0L;
    }
#ifdef __cplusplus
};
#endif

