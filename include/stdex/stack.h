#pragma once

#include <string>

namespace stdex
{
    bool get_callstack( std::string & _message, void * _context = NULL );
}
