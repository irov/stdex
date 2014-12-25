#	include "stdex/exception.h"

#	include <cstdio>
#	include <stdarg.h>

#	include <exception>

namespace stdex
{
	namespace helper
	{
		//////////////////////////////////////////////////////////////////////////
		class stdex_exception
			: public std::exception
		{
		public:
			stdex_exception( const char * _message )
				: m_message(_message)
			{
			}

		protected:
			const char * what() const throw() override
			{
				return m_message;
			}

		protected:
			const char * m_message;
		};
		//////////////////////////////////////////////////////////////////////////
		void throw_exception::operator () ( const char * _format, ... ) const
		{
			va_list argList;

			va_start(argList, _format);

			char msg[2048];

			int msg_err = vsnprintf( msg, 2048 - 1, _format, argList );

			if( msg_err < 0 )
			{
				throw stdex_exception("exception invalid msg formating");
			}

			va_end(argList);

			char msg_fl[2048];
			int msg_fl_err = sprintf(msg_fl, "%s [%s:%d]", msg, file, line);

			if( msg_fl_err < 0 )
			{
				throw stdex_exception("exception invalid msg_fl_err formating");
			}

			throw stdex_exception(msg_fl);
		}
	}
}