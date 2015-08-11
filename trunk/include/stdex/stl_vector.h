#	pragma once

#	include <stdex/stl_allocator.h>
#	include <stdex/stl_allocator_threadsafe.h>

#	include <vector>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	template<class T, class A = stdex::stl_allocator<T> >
	class vector
		: public std::vector<T, A>
	{
	};
	//////////////////////////////////////////////////////////////////////////
	template<class T, class A = stdex::stl_allocator_threadsafe<T> >
	class vector_threadsafe
		: public std::vector<T, A>
	{
	};
}