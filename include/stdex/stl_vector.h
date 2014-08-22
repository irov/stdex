#	pragma once

#	include <stdex/stl_allocator.h>
#	include <vector>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	template<class T, class A = stdex::stl_allocator<T> >
	class vector
		: public std::vector<T, A>
	{
	};
}