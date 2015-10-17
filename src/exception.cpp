#	include "stdex/exception.h"
#	include "stdex/stack.h"

#	include <cstdio>
#	include <stdarg.h>

namespace stdex
{
	namespace helper
	{
		//////////////////////////////////////////////////////////////////////////
		void throw_exception::operator () ( const char * _format, ... ) const
		{
			va_list argList;

			va_start(argList, _format);

			char format_msg[2048] = {0};
			vsnprintf( format_msg, 2048, _format, argList );

			va_end(argList);
			
			stdex::string stack;
			stdex::get_callstack( stack );

			stdex::string message;
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