#	pragma once

#	include <stdint.h>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	namespace helper
	{
		//////////////////////////////////////////////////////////////////////////
		class throw_exception
		{
		public:
			throw_exception( const char * _file, uint32_t _line )
				: file(_file)
				, line(_line)
			{
			}

		public:
			void operator () ( const char * _format, ... );

		public:	
			const char * file;
			uint32_t line;
		};
	}

#	define STDEX_THROW_EXCEPTION\
	stdex::helper::throw_exception(__FILE__, __LINE__)
}