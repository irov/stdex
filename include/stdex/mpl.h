#	pragma once

#	include <stdint.h>

namespace stdex
{
	namespace mpl
	{
		template<class T>
		struct ref_attribute
		{
			typedef const T & type;
		};

		template<class T>
		struct ref_attribute<T *>
		{
			typedef T * type;
		};

		template<>
		struct ref_attribute<bool>
		{
			typedef bool type;
		};

		template<>
		struct ref_attribute<char>
		{
			typedef char type;
		};

		template<>
		struct ref_attribute<wchar_t>
		{
			typedef wchar_t type;
		};

		template<>
		struct ref_attribute<int8_t>
		{
			typedef int8_t type;
		};

		template<>
		struct ref_attribute<int16_t>
		{
			typedef int16_t type;
		};

		template<>
		struct ref_attribute<int32_t>
		{
			typedef int32_t type;
		};

		template<>
		struct ref_attribute<int64_t>
		{
			typedef int64_t type;
		};

		template<>
		struct ref_attribute<uint8_t>
		{
			typedef uint8_t type;
		};

		template<>
		struct ref_attribute<uint16_t>
		{
			typedef uint16_t type;
		};

		template<>
		struct ref_attribute<uint32_t>
		{
			typedef uint32_t type;
		};

		template<>
		struct ref_attribute<uint64_t>
		{
			typedef uint64_t type;
		};

		template<>
		struct ref_attribute<long>
		{
			typedef long type;
		};

		template<>
		struct ref_attribute<unsigned long>
		{
			typedef unsigned long type;
		};

		template<>
		struct ref_attribute<float>
		{
			typedef float type;
		};

		template<>
		struct ref_attribute<double>
		{
			typedef double type;
		};

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

		template<bool C, typename T = void>
		struct enable_if
		{
			typedef T type;
		};

		template<typename T>
		struct enable_if < false, T >
		{
		};

		template<bool C, typename T = void>
		struct disable_if
		{
			typedef T type;
		};

		template<typename T>
		struct disable_if < true, T >
		{
		};

		template<typename, typename>
		struct is_same
		{
			static bool const value = false;
		};

		template<typename A>
		struct is_same < A, A >
		{
			static bool const value = true;
		};

		template<typename B, typename D>
		struct is_base_of
		{
			static D * method();

			static char( &check( B * ) )[1];
			static char( &check( ... ) )[2];

			static const bool value = sizeof check( method() ) == 1 && !is_same < volatile const B, volatile const void > ::value;
		};

		template<class T>
		struct remove_ptr
		{
			typedef T type;
		};

		template<class T>
		struct remove_ptr < T * >
		{
			typedef T type;
		};

		template<class T>
		struct remove_cref
		{
			typedef T type;
		};

		template<class T>
		struct remove_cref<const T &>
		{
			typedef T type;
		};
	}
}