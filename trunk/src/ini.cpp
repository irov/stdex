#   include "stdex/ini.h"

#   include <stdio.h>
#   include <string.h>

namespace stdex
{
	//////////////////////////////////////////////////////////////////////////
	static void s_rtrim( char * s )
	{
		size_t len = strlen(s);

		char * e = s + len - 1;

		while( *e == ' ' || *e == '\t' )
		{
			--e;
		}

		*(e + 1) = '\0';
	}
	//////////////////////////////////////////////////////////////////////////
	static char * s_trim( char * s )
	{
		while( *s == ' ' || *s == '\t' )
		{
			++s;
		}

		s_rtrim( s );

		return s;
	} 
	//////////////////////////////////////////////////////////////////////////
	ini::ini()
		: m_settingsCount(0)
	{
		m_error[0] = '\0';
	}
	//////////////////////////////////////////////////////////////////////////
	bool ini::load( char * _buffer )
	{
		m_settingsCount = 0;

		const char * currentSection = nullptr;
		for( char * line = strtok( _buffer, "\r\n" ); line; line = strtok( nullptr, "\r\n" ) )
		{
			if( this->loadLine_( line, &currentSection ) == false )
			{
				return false;
			}
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ini::loadLine_( char * _line, const char ** _currentSection )
	{
		char * trim_line = s_trim( _line );

		size_t len = strlen( trim_line );

		if( len == 0 )
		{
			return true;
		}

		if( trim_line[0] == ';' )
		{
			return true;
		}

		if( trim_line[0] == '#' )
		{
			return true;
		}

		if( trim_line[0] == '[' && trim_line[len-1] == ']' )
		{   
			char name[64];
			if( sscanf( trim_line, "[%[^]]", name ) != 1 )
			{
				sprintf(m_error, "invalidate parse section: %s"
					, trim_line
					);

				return false;
			}

			char * section_str = strstr( trim_line, name );
			size_t section_len = strlen( name );

			section_str[section_len] = '\0';

			*_currentSection = section_str;

			return true;
		}

		if( *_currentSection == nullptr )
		{
			sprintf(m_error, "first element is not section"
				);

			return false;
		}

		char key[64];
		char value[256];              
		if( sscanf( trim_line, "%[^=] = \"%[^\"]\"", key, value ) == 2
			||  sscanf( trim_line, "%[^=] = '%[^\']'", key, value ) == 2
			||  sscanf( trim_line, "%[^=] = %[^;#]", key, value ) == 2 )         
		{            
			char * key_str = strstr( trim_line, key );
			size_t key_len = strlen( key );
			key_str[key_len] = '\0';
			s_rtrim( key_str );

			char * value_str = strstr( trim_line + key_len + 1, value );
			size_t value_len = strlen( value );
			value_str[value_len] = '\0';
			s_rtrim( value_str );

			bool successful = this->addSetting( *_currentSection, key_str, value_str );

			return successful;
		}

		if( sscanf( trim_line, "%[^=] =", key ) == 1 )
		{
			char * key_str = strstr( trim_line, key );
			size_t key_len = strlen( key );
			key_str[key_len] = '\0';
			s_rtrim( key_str );

			bool successful = this->addSetting( *_currentSection, key_str, key_str + key_len );
						
			return successful;
		}

		return false;
	}    
	//////////////////////////////////////////////////////////////////////////
	bool ini::hasSection( const char * _section ) const
	{
		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	const char * ini::getSettingValue( const char * _section, const char * _key ) const
	{
		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			if( strcmp( setting.key, _key ) != 0 )
			{
				continue;
			}

			const char * value = setting.value;

			return value;
		}

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	uint32_t ini::countSettingValues( const char * _section, const char * _key ) const
	{
		uint32_t count = 0;

		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			if( strcmp( setting.key, _key ) != 0 )
			{
				continue;
			}

			++count;
		}

		return count;
	}
	//////////////////////////////////////////////////////////////////////////
	const char * ini::getSettingValues( const char * _section, const char * _key, uint32_t _index ) const
	{
		uint32_t count = 0;

		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			if( strcmp( setting.key, _key ) != 0 )
			{
				continue;
			}

			if( count != _index )
			{
				++count;

				continue;
			}

			const char * value = setting.value;

			return value;
		}

		return nullptr;
	}
	//////////////////////////////////////////////////////////////////////////
	uint32_t ini::countSettings( const char * _section ) const
	{
		uint32_t count = 0;

		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			++count;
		}

		return count;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ini::addSetting( const char * _section, const char * _key, const char * _value )
	{
		Setting & setting = m_settings[m_settingsCount];

		setting.section = _section;
		setting.key = _key;
		setting.value = _value;

		++m_settingsCount;

		if( m_settingsCount == STDEX_INI_MAX_SETTINGS )
		{
			sprintf(m_error, "elements is MAX %d"
				, STDEX_INI_MAX_SETTINGS
				);

			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ini::mergeSetting( const char * _section, const char * _key, const char * _value )
	{
		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			if( strcmp( setting.key, _key ) != 0 )
			{
				continue;
			}

			setting.value = _value;

			return true;
		}

		Setting & setting = m_settings[m_settingsCount];

		setting.section = _section;
		setting.key = _key;
		setting.value = _value;

		++m_settingsCount;

		if( m_settingsCount == STDEX_INI_MAX_SETTINGS )
		{
			return false;
		}

		return true;
	}
	//////////////////////////////////////////////////////////////////////////
	bool ini::getSettings( const char * _section, uint32_t _index, const char ** _key, const char ** _value ) const
	{
		uint32_t count = 0;

		for( uint32_t index = 0; index != m_settingsCount; ++index )
		{
			const Setting & setting = m_settings[index];

			if( strcmp( setting.section, _section ) != 0 )
			{
				continue;
			}

			if( count != _index )
			{
				++count;

				continue;
			}

			*_key = setting.key;
			*_value = setting.value;

			return true;
		}

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	const char * ini::getError() const
	{
		return m_error;
	}
}