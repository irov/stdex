#	pragma once

#	include <stdint.h>

namespace stdex
{
	class exception
	{
	public:
		exception( const char * _file, size_t _line, const char * _doc )
			: file(_file)
			, line(_line)
			, doc(_doc)
		{
		}

	public:
		const char * file;
		size_t line;
		const char * doc;
	};

#	define STDEX_THROW_EXCEPTION(Doc)\
	throw stdex::exception(__FILE__, __LINE__, Doc)
}