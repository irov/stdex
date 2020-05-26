#	pragma once

#	include <cstdint>

namespace stdex
{
	namespace mpl
	{
		template<class T>
		struct ref_attribute_pod
		{
			typedef T type;
		};

		template<>
		struct ref_attribute_pod<bool>
		{
			typedef bool type;
		};

		template<>
		struct ref_attribute_pod<char>
		{
			typedef char type;
		};

		template<>
		struct ref_attribute_pod<wchar_t>
		{
			typedef wchar_t type;
		};

		template<>
		struct ref_attribute_pod<int8_t>
		{
			typedef int8_t type;
		};

		template<>
		struct ref_attribute_pod<int16_t>
		{
			typedef int16_t type;
		};

		template<>
		struct ref_attribute_pod<int32_t>
		{
			typedef int32_t type;
		};

		template<>
		struct ref_attribute_pod<int64_t>
		{
			typedef int64_t type;
		};

		template<>
		struct ref_attribute_pod<uint8_t>
		{
			typedef uint8_t type;
		};

		template<>
		struct ref_attribute_pod<uint16_t>
		{
			typedef uint16_t type;
		};

		template<>
		struct ref_attribute_pod<uint32_t>
		{
			typedef uint32_t type;
		};

		template<>
		struct ref_attribute_pod<uint64_t>
		{
			typedef uint64_t type;
		};

		template<>
		struct ref_attribute_pod<float>
		{
			typedef float type;
		};

		template<>
		struct ref_attribute_pod<double>
		{
			typedef double type;
		};

		template<class T>
		struct ref_attribute
		{
			typedef T type;
		};

		template<class T>
		struct ref_attribute<const T &>
		{
			typedef typename ref_attribute_pod<T>::type type;
		};

		template<class T>
		struct ref_attribute<T *>
		{
			typedef T * type;
		};

        template<class T, bool D = std::is_polymorphic<typename std::remove_pointer<T>::type>::value>
		struct is_dynamic_cast;

		template<class T>
		struct is_dynamic_cast<T, std::false_type::value>
		{
            template<class U>
			static bool test( U _pointer )
			{
				return static_cast<T>(_pointer) != nullptr;
			}
		};

		template<class T>
		struct is_dynamic_cast<T, std::true_type::value>
		{
            template<class U>
            static bool test( U _pointer )
			{
                return dynamic_cast<T>(_pointer) != nullptr;
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

		template<class T>
		struct remove_modify
		{
			typedef T type;
		};

		template<class T>
		struct remove_modify < T * >
		{
			typedef T type;
		};

		template<class T>
		struct remove_modify<const T &>
		{
			typedef T type;
		};
	}
}