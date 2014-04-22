//////////////////////////////////////////////////////////////////////////
template<int s00, int s01, int s02, int s03, int s04, int s05, int s06, int s07, int s08, int s09>
const ConstString & stringizeStringLocal( const char * _str, size_t _size )
{ 
	(void)s00;
	(void)s01;
	(void)s02;
	(void)s03;
	(void)s04;
	(void)s05;
	(void)s06;
	(void)s07;
	(void)s08;
	(void)s09;

	static bool initialize = false;
	static ConstStringHolderLocal c_holder_local;
	static ConstStringHolderLocal c_str;	
	if( initialize == false )
	{ 
		initialize = true;
		c_holder_local.setup( _str, _size );
		c_str = ConstString(&c_holder_local);
	}

	return c_str;
}
	}

#	define CONST_STRING_II(II)\
#II

#	define CONST_STRING_I(c00, c01, c02, c03, c04, c05, c06, c07, c08, c09)\
	CONST_STRING_II(c00##c01##c02##c03##c04##c05##c06##c07##c08##c09)

#	define CONST_CHAR_I(c)\
#@c

#	define CONST_STRING_10(c00, c01, c02, c03, c04, c05, c06, c07, c08, c09)\
	Helper::stringizeStringLocal<\
	CONST_CHAR_I(c00), CONST_CHAR_I(c01), CONST_CHAR_I(c02), CONST_CHAR_I(c03), CONST_CHAR_I(c04), CONST_CHAR_I(c05), CONST_CHAR_I(c06), CONST_CHAR_I(c07), CONST_CHAR_I(c08), CONST_CHAR_I(c09)\
	>\
	(\
	CONST_STRING_I(c00, c01, c02, c03, c04, c05, c06, c07, c08, c09)\
	, sizeof( CONST_STRING_I(c00, c01, c02, c03, c04, c05, c06, c07, c08, c09) )\
	)

#	define CONST_STRING_9(c00, c01, c02, c03, c04, c05, c06, c07, c08)\
	CONST_STRING_10(c00, c01, c02, c03, c04, c05, c06, c07, c08, _)

#	define CONST_STRING_8(c00, c01, c02, c03, c04, c05, c06, c07)\
	CONST_STRING_9(c00, c01, c02, c03, c04, c05, c06, c07, _)

#	define CONST_STRING_7(c00, c01, c02, c03, c04, c05, c06)\
	CONST_STRING_8(c00, c01, c02, c03, c04, c05, c06, _)

#	define CONST_STRING_6(c00, c01, c02, c03, c04, c05)\
	CONST_STRING_7(c00, c01, c02, c03, c04, c05, _)

#	define CONST_STRING_5(c00, c01, c02, c03, c04)\
	CONST_STRING_6(c00, c01, c02, c03, c04, _)

#	define CONST_STRING_4(c00, c01, c02, c03)\
	CONST_STRING_5(c00, c01, c02, c03, _)

#	define CONST_STRING_3(c00, c01, c02)\
	CONST_STRING_4(c00, c01, c02, _)

#	define CONST_STRING_2(c00, c01)\
	CONST_STRING_3(c00, c01, _)

#	define CONST_STRING_1(c00)\
	CONST_STRING_2(c00, _)