#	pragma once

#	include <stdint.h>

#	include <exception>

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
			stdex_exception( const char * _message )
				: m_message( _message )
			{
			}

			~stdex_exception() throw()
			{
			}

		protected:
			const char * what() const throw() override
			{
				return m_message;
			}

		protected:
			const char * m_message;
		};
		//////////////////////////////////////////////////////////////////////////
		class throw_exception
		{
		public:
			throw_exception( const char * _file, uint32_t _line )
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