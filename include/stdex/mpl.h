#	pragma once

namespace stdex
{
	template<class T>
	struct is_dynamic_castable
	{
		template<class U>
		static char test( T t = dynamic_cast<T>(nullptr) );

		static double test( ... );

		enum
		{
			result = sizeof( test( (T)nullptr ) ) == sizeof( char )
		};
	};

	template<class T, bool D = is_dynamic_castable<T>::result>
	struct is_dynamic_cast;

	template<class T>
	struct is_dynamic_cast<T, false>
	{
		static bool test( void * )
		{
			return true;
		}
	};

	template<class T>
	struct is_dynamic_cast<T, true>
	{
		static bool test( void * _pointer )
		{
			return dynamic_cast<T>(_pointer) == nullptr;
		}
	};
}