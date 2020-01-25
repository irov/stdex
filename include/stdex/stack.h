#pragma once

#include <string>

namespace stdex
{
    //////////////////////////////////////////////////////////////////////////
    typedef void (*log_callstack_t)(void * _ud, const char * _format, ...);
    //////////////////////////////////////////////////////////////////////////
    bool get_callstack( std::string & _stack, log_callstack_t _log, void * _ud, void * _context = NULL );
}
