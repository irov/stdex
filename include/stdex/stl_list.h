#	pragma once

#	include <stdex/stl_allocator.h>
#	include <list>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	template<class T, class A = stdex::stl_allocator<T> >
	class list
		: public std::list<T, A>
	{
	};
}