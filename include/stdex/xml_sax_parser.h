#	pragma once

#	include <string.h>
#	include <stdint.h>

namespace stdex
{
	namespace detail
	{
		//////////////////////////////////////////////////////////////////////////
		struct xml_sax_parse_attribute
		{
			uint32_t count;

			const char * key[64];
			const char * value[64];			
		};
		//////////////////////////////////////////////////////////////////////////
		inline static char * str_unfind( char * s, char ch )
		{
			char * e = s;
			while( *e && *e == ch )
			{
				++e;
			}

			return e;
		}
		//////////////////////////////////////////////////////////////////////////
		char * s_xml_parse_node_attribute( xml_sax_parse_attribute & _attr, char * _attribute, const char * _node );
		//////////////////////////////////////////////////////////////////////////
		template<class t_xml_sax_callback>
		static char * s_xml_parse_node( t_xml_sax_callback & _callback, char * _node )
		{
			char * begin_name_node = str_unfind( _node, ' ' );

			if( *begin_name_node == '/' )
			{
				char * begin_name_node_end = str_unfind( begin_name_node + 1, ' ' );
				char * end_name_node_end = strpbrk( begin_name_node, " >" );

				if( *end_name_node_end == ' ' )
				{
					*end_name_node_end = '\0';

					_callback.callback_end_node( begin_name_node_end );

					char * end_name_node2 = strchr( begin_name_node_end + 1, '>' );

					return end_name_node2 + 1;
				}
				else if( *end_name_node_end == '>' )
				{
					*end_name_node_end = '\0';

					_callback.callback_end_node( begin_name_node_end );

					return end_name_node_end + 1;
				}
			}
			else if( *begin_name_node == '!' )
			{
				char * end_name_node2 = strstr( begin_name_node + 1, "-->" );

				return end_name_node2 + 3;
			}
			else if( *begin_name_node == '?' )
			{
				char * end_name_node2 = strstr( begin_name_node + 1, "?>" );

				return end_name_node2 + 2;
			}

			char * end_name_node = strpbrk( begin_name_node, " />" );

			if( *end_name_node == ' ' )
			{
				*end_name_node = '\0';

				_callback.callback_begin_node( begin_name_node );

				char * find_node_attribute = str_unfind( end_name_node + 1, ' ' );

				if( *find_node_attribute == '>' )
				{
					return find_node_attribute + 1;
				}
				else if( *find_node_attribute == '/' )
				{					
					_callback.callback_end_node( begin_name_node );

					char * end_name_node2 = strchr( find_node_attribute + 1, '>' );

					return end_name_node2 + 1;
				}

				xml_sax_parse_attribute attr;
				attr.count = 0;

				char * end_node_attribute = s_xml_parse_node_attribute( attr, find_node_attribute, begin_name_node );

				_callback.callback_node_attributes( begin_name_node, attr.count, attr.key, attr.value );

				if( *end_node_attribute == '>' )
				{
					return end_node_attribute + 1;
				}
				else if( *end_node_attribute == '/' )
				{
					_callback.callback_end_node( begin_name_node );

					char * end_node_attribute2 = strchr( end_node_attribute + 1, '>' );

					return end_node_attribute2 + 1;
				}

				return end_node_attribute;
			}
			else if( *end_name_node == '>' )
			{
				*end_name_node = '\0';

				_callback.callback_begin_node( begin_name_node );

				return end_name_node + 1;
			}
			else if( *end_name_node == '/' ) 
			{
				*end_name_node = '\0';

				_callback.callback_begin_node( begin_name_node );
				_callback.callback_end_node( begin_name_node );

				char * end_name_node2 = strchr( end_name_node + 1, '>' );
				return end_name_node2 + 1;
			}

			return 0;
		}
	}
	//////////////////////////////////////////////////////////////////////////
	template<class t_xml_sax_callback>
	bool xml_sax_parse( char * _xml, t_xml_sax_callback & _callback )
	{
		char * begin_node = _xml;
		
		uint32_t BOM = 0;
		BOM |= (unsigned char)(begin_node[0]) << 16;
		BOM |= (unsigned char)(begin_node[1]) << 8;
		BOM |= (unsigned char)(begin_node[2]);

		if( BOM == 0x00EFBBBF )
		{
			begin_node += 3;
		}

		for(;;)
		{
			begin_node = strchr( begin_node, '<' );

			if( begin_node == nullptr )
			{
				break;
			}

			begin_node = detail::s_xml_parse_node<t_xml_sax_callback>( _callback, begin_node + 1 );
		}

		return true;
	}
}
