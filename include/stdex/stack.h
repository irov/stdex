#	pragma once

#	include "stdex/stl_string.h"

namespace stdex
{
	bool get_callstack( stdex::string & _message, void * _context = NULL );
}
