#	include "stdex/xml_sax_parser.h"

namespace stdex
{
	namespace detail
	{

		//////////////////////////////////////////////////////////////////////////
		const unsigned char xml_lookup_digits_tables[256] = 
		{
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			0,  1,  2,  3,  4,  5,  6,  7,  8,  9,255,255,255,255,255,255,
			255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255, 10, 11, 12, 13, 14, 15,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,
			255,255,255,255,255,255,255,255,255,255,255,255,255,255,255,255
		};
		//////////////////////////////////////////////////////////////////////////
		static bool s_xml_adapt_value_attribute( char * _value )
		{
			char * src = _value;

			for(;;)
			{
				src = strchr( src, '&' );

				if( src == nullptr )
				{
					return true;
				}

				switch( src[1] )
				{				
				case 'a': // &amp; &apos;
					{
						if( src[2] == 'm' && src[3] == 'p' && src[4] == ';' )
						{
							*src = '&';
							strcpy( src + 1, src + 5 );
							src += 1;
						}
						else if( src[2] == 'p' && src[3] == 'o' && src[4] == 's' && src[5] == ';' )
						{
							*src = '\'';
							strcpy( src + 1, src + 6 );
							src += 1;
						}
						else
						{
							return false;
						}
					}break;
				case 'q': // &quot;
					{
						if( src[2] == 'u' && src[3] == 'o' && src[4] == 't' && src[5] == ';' )
						{
							*src = '"';
							strcpy( src + 1, src + 6 );
							src += 1;
						}
						else
						{
							return false;
						}
					}break;
				case 'g': // &gt;
					{
						if( src[2] == 't' && src[3] == ';' )
						{
							*src = '>';
							strcpy( src + 1, src + 4 );
							src += 1;
						}
						else
						{
							return false;
						}
					}break;
				case 'l': // &lt;
					{
						if( src[2] == 't' && src[3] == ';' )
						{
							*src = '<';
							strcpy( src + 1, src + 4 );
							src += 1;
						}
						else
						{
							return false;
						}
					}break;
				case 'n': // &nbsp;
					{
						if( src[2] == 'b' && src[3] == 's' && src[4] == 'p' && src[5] == ';' )
						{
							*(src + 0) = 0xC2;
							*(src + 1) = 0xA0;
							strcpy( src + 2, src + 6 );
							src += 2;
						}
						else
						{
							return false;
						}
					}break;
				case '#':
					{
						unsigned long code = 0;
						const char * src_code = src;

						if( src[2] == 'x' )
						{						
							src_code += 3;
							for(;;)
							{
								unsigned char table_code = static_cast<unsigned char>(*src_code);
								unsigned char digit = xml_lookup_digits_tables[table_code];
								if( digit == 0xFF )
								{
									break;
								}

								code = code * 16 + digit;
								++src_code;
							}				
						}
						else
						{
							src_code += 2;
							for(;;)
							{
								unsigned char table_code = static_cast<unsigned char>(*src_code);
								unsigned char digit = xml_lookup_digits_tables[table_code];
								if( digit == 0xFF )
								{
									break;
								}

								code = code * 10 + digit;
								++src_code;
							}

							if( *src_code != ';' )
							{
								return false;
							}
						}

						if( code < 0x80 )    // 1 byte sequence
						{
							src[0] = static_cast<unsigned char>(code);

							src += 1;
						}
						else if( code < 0x800 )  // 2 byte sequence
						{
							src[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[0] = static_cast<unsigned char>(code | 0xC0);

							src += 2;						
						}
						else if( code < 0x10000 )    // 3 byte sequence
						{
							src[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[0] = static_cast<unsigned char>(code | 0xE0);

							src += 3;					
						}
						else if (code < 0x110000)   // 4 byte sequence
						{
							src[3] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[2] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[1] = static_cast<unsigned char>((code | 0x80) & 0xBF); code >>= 6;
							src[0] = static_cast<unsigned char>(code | 0xF0);

							src += 4;
						}
						else
						{
							return false;
						}

						strcpy( src, src_code + 1 );
					}break;
				}
			}

			return true;
		}
		//////////////////////////////////////////////////////////////////////////
		char * s_xml_parse_node_attribute( xml_sax_parse_attribute & _attr, char * _attribute, const char * _node )
		{
			char * end_name_node_attribute = strpbrk( _attribute, " =" );

			if( *end_name_node_attribute == ' ' )
			{
				*end_name_node_attribute = '\0';
			}
			else if( *end_name_node_attribute == '=' )
			{
				*end_name_node_attribute = '\0';
			}

			char * begin_value_node_attribute = strchr( end_name_node_attribute + 1, '"' );
			char * end_value_node_attribute = strchr( begin_value_node_attribute + 1, '"' );

			*end_value_node_attribute = '\0';

			if( s_xml_adapt_value_attribute( begin_value_node_attribute + 1 ) == false )
			{
				return 0;
			}

			_attr.key[_attr.count] = _attribute;
			_attr.value[_attr.count] = begin_value_node_attribute + 1;
			++_attr.count;

			char * find_next_node_attribute = str_unfind( end_value_node_attribute + 1, ' ' );

			if( *find_next_node_attribute == '>' )
			{
				return find_next_node_attribute;
			}
			else if( *find_next_node_attribute == '/' )
			{
				return find_next_node_attribute;
			}

			char * end_node_attribute = s_xml_parse_node_attribute( _attr, find_next_node_attribute, _node );

			return end_node_attribute;
		}
	}
}