#pragma once

#include "stdex/thread_guard.h"

namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    class thread_guard_scope
    {
    public:
        thread_guard_scope( const thread_guard & _guard, const char * _file, uint32_t _line, const char * _doc );
        ~thread_guard_scope();

    private:
        thread_guard_scope( const thread_guard_scope & );
        void operator = ( const thread_guard_scope & );

    protected:
        const thread_guard & m_guard;

        const char * m_file;
        uint32_t m_line;
        const char * m_doc;
    };
    //////////////////////////////////////////////////////////////////////////
#   ifdef STDEX_THREAD_GUARD
    //////////////////////////////////////////////////////////////////////////
#   define STDEX_THREAD_GUARD_SCOPE(Self, Doc)\
    stdex::thread_guard_scope stdex_thread_guard_scope(Self->m_stdex_thread_guard, __FILE__, __LINE__, Doc )
    //////////////////////////////////////////////////////////////////////////
#   else
    //////////////////////////////////////////////////////////////////////////
#   define STDEX_THREAD_GUARD_SCOPE(Self, Doc)
    //////////////////////////////////////////////////////////////////////////
#   endif
}

