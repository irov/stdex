#pragma once

#include "stdex/stl_allocator.h"

#include <list>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	template<class T, class A = stdex::stl_allocator<T> >
    using list = std::list<T, A>;
}