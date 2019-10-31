#include "stdex/thread_guard.h"

#ifdef WIN32

#ifdef _WIN32_WINNT	
#   undef _WIN32_WINNT
#   define _WIN32_WINNT 0x0500
#endif

#ifdef _WIN32_WINDOWS
#   undef _WIN32_WINDOWS
#   define _WIN32_WINDOWS 0x0500
#endif

#define WIN32_LEAN_AND_MEAN

#ifndef NOMINMAX
#	define NOMINMAX
#endif

#pragma warning(push, 0) 
#include <Windows.h>
#  include <WinUser.h>

#include <shellapi.h>
#include <shlobj.h>
#pragma warning(pop)

#endif

#ifdef WIN32
#   define STDEX_THREAD_GUARD_GET_CURRENT_THREAD_ID() ((ptrdiff_t)GetCurrentThreadId())
#else
#   define STDEX_THREAD_GUARD_GET_CURRENT_THREAD_ID() (0U)
#endif
//////////////////////////////////////////////////////////////////////////
namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    thread_guard::thread_guard()
        : m_lock( false )
    {
        m_id = STDEX_THREAD_GUARD_GET_CURRENT_THREAD_ID();
    }
    //////////////////////////////////////////////////////////////////////////
    thread_guard::~thread_guard()
    {
    }
    //////////////////////////////////////////////////////////////////////////
    void thread_guard::reset()
    {
        m_id = 0;
    }
    //////////////////////////////////////////////////////////////////////////
    void thread_guard::check( const char * _file, uint32_t _line, const char * _doc ) const
    {
        uint32_t id = STDEX_THREAD_GUARD_GET_CURRENT_THREAD_ID();

        if( m_id == id || m_id == 0 )
        {
            return;
        }

#if defined(WIN32) && !defined(NDEBUG)
        unsigned int * p = nullptr;
        *p = 0xBADF00D;
#endif

        stdex::helper::throw_exception( _file, _line )(_doc);
    }
    //////////////////////////////////////////////////////////////////////////
    bool thread_guard::lock( bool _value ) const
    {
        bool lock = m_lock;

        m_lock = _value;

        return lock;
    }
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