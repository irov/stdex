#	pragma once

#	include <stdex/stl_allocator.h>
#	include <map>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	template<class K, class T, class L = std::less<K>, class A = stdex::stl_allocator<std::pair<const K, T> > >
	class map
		: public std::map<K, T, L, A>
	{
	};
}