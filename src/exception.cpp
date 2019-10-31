#include "stdex/exception.h"
#include "stdex/stack.h"

#include <cstdio>
#include <string>
#include <stdarg.h>

namespace stdex
{
    namespace helper
    {
        //////////////////////////////////////////////////////////////////////////
        stdex_exception::stdex_exception( const std::string & _message ) noexcept
            : m_msg( _message )
        {
        }
        //////////////////////////////////////////////////////////////////////////
        stdex_exception::~stdex_exception() noexcept
        {
        }
        //////////////////////////////////////////////////////////////////////////
        const char * stdex_exception::what() const noexcept
        {
            return m_msg.c_str();
        }
        //////////////////////////////////////////////////////////////////////////
        void throw_exception::operator () ( const char * _format, ... ) const
        {
            va_list argList;

            va_start( argList, _format );

            char format_msg[4096] = {0};
            vsprintf( format_msg, _format, argList );

            va_end( argList );

            std::string stack;
            stdex::get_callstack( stack );

            std::string message;
            message += "message: ";
            message += format_msg;
            message += "\n";

            message += "file: ";
            message += m_file;
            message += "\n";

            char format_line[16] = {0};
            sprintf( format_line, "%d", m_line );

            message += "line: ";
            message += format_line;
            message += "\n";

            message += "stack:\n";
            message += stack;

            const char * c_msg = message.c_str();

            throw stdex_exception( c_msg );
        }
    }
}