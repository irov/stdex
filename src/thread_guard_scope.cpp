#include "stdex/thread_guard_scope.h"

//////////////////////////////////////////////////////////////////////////
namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    thread_guard_scope::thread_guard_scope( const thread_guard & _guard, const char * _file, uint32_t _line, const char * _doc )
        : m_guard( _guard )
        , m_file( _file )
        , m_line( _line )
        , m_doc( _doc )
    {
        if( m_guard.lock( true ) == true )
        {
#if defined(WIN32) && !defined(NDEBUG)
            unsigned int * p = nullptr;
            *p = 0xBADF00D;
#endif

            stdex::helper::throw_exception( m_file, m_line )(m_doc);
        }
    }
    //////////////////////////////////////////////////////////////////////////
    thread_guard_scope::~thread_guard_scope()
    {
        if( m_guard.lock( false ) == false )
        {
#if defined(WIN32) && !defined(NDEBUG)
            unsigned int * p = nullptr;
            *p = 0xBADF00D;
#endif

            stdex::helper::throw_exception( m_file, m_line )(m_doc);
        }
    }
}