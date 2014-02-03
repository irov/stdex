#   pragma once

#   include <stddef.h>

namespace stdex
{    
	//////////////////////////////////////////////////////////////////////////
	class ini
	{
	public:
		ini();

	public:
		bool load( char * _buffer );

	public:
		bool hasSection( const char * _section ) const;

	public:
		const char * getSettingValue( const char * _section, const char * _key ) const;

	public:
		size_t countSettingValues( const char * _section, const char * _key ) const;
		const char * getSettingValues( const char * _section, const char * _key, size_t _index ) const;

	public:
		size_t countSettings( const char * _section ) const;
		bool getSettings( const char * _section, size_t _index, const char ** _key, const char ** _value ) const;

	protected:
		bool loadLine_( char * _buf );       

	protected:        
		const char * m_currentSection;

		struct Setting
		{
			const char * section;
			const char * key;
			const char * value;
		};

		Setting m_settings[256];
		size_t m_settingsCount;
	};
}