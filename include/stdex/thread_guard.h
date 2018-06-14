#pragma once

#include "stdex/exception.h"

#include <atomic>

#include <stdint.h>

#ifndef STDEX_THREAD_GUARD_ENABLE
#	ifndef NDEBUG
#	    define STDEX_THREAD_GUARD
#   endif
#else
#	define STDEX_THREAD_GUARD
#endif

//////////////////////////////////////////////////////////////////////////
namespace stdex
{	
	//////////////////////////////////////////////////////////////////////////
	class thread_guard
	{
	public:
		thread_guard();
		~thread_guard();

	public:
		void reset();
		void check( const char * _file, uint32_t _line, const char * _doc ) const;
		bool lock( bool _value ) const;

	protected:
		std::atomic_uint32_t m_id;
		mutable std::atomic_bool m_lock;
	};
	//////////////////////////////////////////////////////////////////////////
	class thread_guard_scope
	{
	public:
		thread_guard_scope( const thread_guard & _guard, const char * _file, uint32_t _line, const char * _doc );
		~thread_guard_scope();

	private:
		void operator = (const thread_guard_scope &)
		{
		};

	protected:
		const thread_guard & m_guard;

		const char * m_file;
		uint32_t m_line;
		const char * m_doc;
	};
	//////////////////////////////////////////////////////////////////////////
#	ifdef STDEX_THREAD_GUARD
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_INIT\
	public:\
	stdex::thread_guard m_stdex_thread_guard\
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_CHECK(Self, Doc)\
	Self->m_stdex_thread_guard.check( __FILE__, __LINE__, Doc )
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_SCOPE(Self, Doc)\
	stdex::thread_guard_scope stdex_thread_guard_scope(Self->m_stdex_thread_guard, __FILE__, __LINE__, Doc )
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_RESET(Self)\
	Self->m_stdex_thread_guard.reset()
	//////////////////////////////////////////////////////////////////////////
#	else
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_INIT
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_CHECK(Self, Doc)
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_SCOPE(Self, Doc)
	//////////////////////////////////////////////////////////////////////////
#	define STDEX_THREAD_GUARD_RESET(Self)
	//////////////////////////////////////////////////////////////////////////
#	endif
} 

