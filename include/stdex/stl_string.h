#	pragma once

#	include <stdex/stl_allocator.h>
#	include <string>

namespace stdex
{
	typedef std::basic_string<char, std::char_traits<char>, stdex::stl_allocator<char> > string;
	typedef std::basic_string<wchar_t, std::char_traits<wchar_t>, stdex::stl_allocator<wchar_t> > wstring;
    typedef std::basic_string<uint32_t, std::char_traits<uint32_t>, stdex::stl_allocator<uint32_t> > u32string;
}