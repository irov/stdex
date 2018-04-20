#include "stdex/allocator_report.h"

#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif
    //////////////////////////////////////////////////////////////////////////
    stdex_memory_report_t * mm = nullptr;
    //////////////////////////////////////////////////////////////////////////
    static void stdex_allocator_report( size_t _incref, size_t _decref, const char * _name )
    {
        (void)_incref;
        (void)_decref;
        (void)_name;

#ifdef STDEX_ALLOCATOR_REPORT_ENABLE
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
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_new( size_t _size, const char * _name )
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

        stdex_allocator_report( 0U, ptr_size, _name );
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    void stdex_allocator_report_realoc( void * _ptr, size_t _size, const char * _name )
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

            stdex_allocator_report( _size, ptr_size, _name );
        }
#endif
    }
    //////////////////////////////////////////////////////////////////////////
    size_t stdex_allocator_report_count()
    {
        if( mm == nullptr )
        {
            return 0;
        }

        size_t u = 0;

        for( size_t i = 0; i != STDEX_ALLOCATOR_REPORT_MAX_COUNT; ++i )
        {
            stdex_memory_report_t * m = mm + i;

            if( m->name == 0 )
            {
                break;
            }

            ++u;
        }

        return u;
    }
    //////////////////////////////////////////////////////////////////////////
    struct stdex_memory_report_t * stdex_allocator_report_info( size_t _index )
    {
        return mm + _index;
    }
#ifdef __cplusplus
};
#endif

