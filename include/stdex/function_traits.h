#pragma once

#include <tuple>

#include <stdint.h>

namespace stdex
{
	template<class T>
	struct external_type_cast
	{
		typedef T Type;
	};

	namespace detail
	{
        template<uint32_t I>
        struct void_size_t
        {
        };

        template<bool Cond, template <uint32_t> class T = void_size_t, uint32_t I = 0> struct function_types_args
        {
            typedef void type;
        };

        template<template <uint32_t> class T, uint32_t I> struct function_types_args<true, T, I>
        {
            typedef T<I> type;
        };

		template<class T>
		struct function_types;

        template<class R, class ... Args>
        struct function_types< R( *)(Args ...) >
		{
			typedef R ret_type;
			typedef void class_type;

            typedef std::tuple<Args...> params;

            template<uint32_t I>
            using param = typename external_type_cast<typename std::tuple_element<I, std::tuple<Args...> >::type>::Type;

			static const bool method = false;
			static const uint32_t arity = sizeof ... (Args);

            typedef typename function_types_args<arity != 0, param, 0>::type first_param;
            typedef typename function_types_args<arity != 0, param, arity - 1>::type last_param;
		};

		template<class R, class C, class ... Args>
		struct function_types< R( C::* )(Args ...) >
		{
			typedef R ret_type;
			typedef C class_type;

            typedef std::tuple<Args...> params;

            template<uint32_t I>
            using param = typename external_type_cast<typename std::tuple_element<I, std::tuple<Args...> >::type>::Type;

			static const bool method = true;
			static const uint32_t arity = sizeof ... (Args);

            typedef typename function_types_args<arity != 0, param, 0>::type first_param;
            typedef typename function_types_args<arity != 0, param, arity - 1>::type last_param;
		};

        template<class R, class C, class ... Args>
        struct function_types< R( C::* )(Args ...) const >
        {
            typedef R ret_type;
            typedef C class_type;

            typedef std::tuple<Args...> params;

            template<uint32_t I>
            using param = typename external_type_cast<typename std::tuple_element<I, std::tuple<Args...> >::type>::Type;

            static const bool method = true;
            static const uint32_t arity = sizeof ... (Args);

            typedef typename function_types_args<arity != 0, param, 0>::type first_param;
            typedef typename function_types_args<arity != 0, param, arity - 1>::type last_param;
        };

#if defined(_MSC_VER) || defined(__MINGW32__) || defined(__ANDROID__)
        template<class R, class C, class ... Args>
        struct function_types< R( C::* )(Args ...) noexcept >
        {
            typedef R ret_type;
            typedef C class_type;

            typedef std::tuple<Args...> params;

            template<uint32_t I>
            using param = typename external_type_cast<typename std::tuple_element<I, std::tuple<Args...> >::type>::Type;

            static const bool method = true;
            static const uint32_t arity = sizeof ... (Args);

            typedef typename function_types_args<arity != 0, param, 0>::type first_param;
            typedef typename function_types_args<arity != 0, param, arity - 1>::type last_param;
        };

        template<class R, class C, class ... Args>
        struct function_types< R( C::* )(Args ...) const noexcept>
        {
            typedef R ret_type;
            typedef C class_type;

            typedef std::tuple<Args...> params;

            template<uint32_t I>
            using param = typename external_type_cast<typename std::tuple_element<I, std::tuple<Args...> >::type>::Type;

            static const bool method = true;
            static const uint32_t arity = sizeof ... (Args);

            typedef typename function_types_args<arity != 0, param, 0>::type first_param;
            typedef typename function_types_args<arity != 0, param, arity - 1>::type last_param;
        };
#endif
	}

	template<class F>
	struct function_traits
	{
		typedef detail::function_types<F> result;
	};
}

