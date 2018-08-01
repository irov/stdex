#pragma once

#include <stdint.h>

#include <exception>
#include <string>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	namespace helper
	{
		//////////////////////////////////////////////////////////////////////////
		class stdex_exception
			: public std::exception
		{
		public:
			explicit stdex_exception( const std::string & _message ) noexcept;
			~stdex_exception() noexcept;

		public:
			const char* what() const noexcept override;

		protected:
			std::string m_msg;
		};
		//////////////////////////////////////////////////////////////////////////
		class throw_exception
		{
		public:
			throw_exception( const char * _file, uint32_t _line ) noexcept
				: m_file(_file)
				, m_line( _line )
			{
			}

		public:
			void operator () ( const char * _format, ... ) const;

		protected:
			const char * m_file;
			uint32_t m_line;
		};
	}

#	define STDEX_THROW_EXCEPTION\
	stdex::helper::throw_exception(__FILE__, __LINE__)
}