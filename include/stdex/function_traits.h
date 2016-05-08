#	pragma once

#	include <stdint.h>

namespace stdex
{
	template<class T>
	struct external_type_cast
	{
		typedef T Type;
	};

	namespace detail
	{
		template<class T>
		struct function_types;

		template<class R>
		struct function_types< R( *)() >
		{
			typedef R ret_type;
			typedef void class_type;

			static const bool method = false;
			static const size_t arity = 0;
		};

		template<class R, class P0>
		struct function_types< R( *)(P0) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;

			static const bool method = false;
			static const size_t arity = 1;
		};

		template<class R, class P0, class P1>
		struct function_types< R( *)(P0, P1) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;

			static const bool method = false;
			static const size_t arity = 2;
		};

		template<class R, class P0, class P1, class P2>
		struct function_types< R( *)(P0, P1, P2) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;

			static const bool method = false;
			static const size_t arity = 3;
		};

		template<class R, class P0, class P1, class P2, class P3>
		struct function_types< R( *)(P0, P1, P2, P3) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;

			static const bool method = false;
			static const size_t arity = 4;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4>
		struct function_types< R( *)(P0, P1, P2, P3, P4) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;

			static const bool method = false;
			static const size_t arity = 5;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;

			static const bool method = false;
			static const size_t arity = 6;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;

			static const bool method = false;
			static const size_t arity = 7;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6, P7) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;

			static const bool method = false;
			static const size_t arity = 8;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6, P7, P8) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;


			static const bool method = false;
			static const size_t arity = 9;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;

			static const bool method = false;
			static const size_t arity = 10;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;

			static const bool method = false;
			static const size_t arity = 11;
		};

		template<class R, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
		struct function_types< R( *)(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) >
		{
			typedef R ret_type;
			typedef void class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;
			typedef typename external_type_cast<P11>::Type param11;

			static const bool method = false;
			static const size_t arity = 12;
		};

		template<class R, class C>
		struct function_types< R( C::* )() >
		{
			typedef R ret_type;
			typedef C class_type;

			static const bool method = true;
			static const size_t arity = 0;
		};

		template<class R, class C>
		struct function_types< R( C::* )() const>
		{
			typedef R ret_type;
			typedef C class_type;

			static const bool method = true;
			static const size_t arity = 0;
		};

		template<class R, class C, class P0>
		struct function_types< R( C::* )(P0) >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;

			static const bool method = true;
			static const size_t arity = 1;
		};

		template<class R, class C, class P0>
		struct function_types< R( C::* )(P0) const >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;

			static const bool method = true;
			static const size_t arity = 1;
		};

		template<class R, class C, class P0, class P1>
		struct function_types< R( C::* )(P0, P1) >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;

			static const bool method = true;
			static const size_t arity = 2;
		};

		template<class R, class C, class P0, class P1>
		struct function_types< R( C::* )(P0, P1) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;

			static const bool method = true;
			static const size_t arity = 2;
		};

		template<class R, class C, class P0, class P1, class P2>
		struct function_types< R( C::* )(P0, P1, P2) >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;

			static const bool method = true;
			static const size_t arity = 3;
		};

		template<class R, class C, class P0, class P1, class P2>
		struct function_types< R( C::* )(P0, P1, P2) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;

			static const bool method = true;
			static const size_t arity = 3;
		};

		template<class R, class C, class P0, class P1, class P2, class P3>
		struct function_types< R( C::* )(P0, P1, P2, P3) >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;

			static const bool method = true;
			static const size_t arity = 4;
		};

		template<class R, class C, class P0, class P1, class P2, class P3>
		struct function_types< R( C::* )(P0, P1, P2, P3) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;

			static const bool method = true;
			static const size_t arity = 4;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4) >
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;

			static const bool method = true;
			static const size_t arity = 5;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;

			static const bool method = true;
			static const size_t arity = 5;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;

			static const bool method = true;
			static const size_t arity = 6;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;

			static const bool method = true;
			static const size_t arity = 6;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;

			static const bool method = true;
			static const size_t arity = 7;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;

			static const bool method = true;
			static const size_t arity = 7;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;

			static const bool method = true;
			static const size_t arity = 8;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;

			static const bool method = true;
			static const size_t arity = 8;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;

			static const bool method = true;
			static const size_t arity = 9;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;

			static const bool method = true;
			static const size_t arity = 9;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;

			static const bool method = true;
			static const size_t arity = 10;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;

			static const bool method = true;
			static const size_t arity = 10;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;

			static const bool method = true;
			static const size_t arity = 11;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;

			static const bool method = true;
			static const size_t arity = 11;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;
			typedef typename external_type_cast<P11>::Type param11;

			static const bool method = true;
			static const size_t arity = 12;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;
			typedef typename external_type_cast<P11>::Type param11;

			static const bool method = true;
			static const size_t arity = 12;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11, class P12>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12)>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;
			typedef typename external_type_cast<P11>::Type param11;
			typedef typename external_type_cast<P12>::Type param12;

			static const bool method = true;
			static const size_t arity = 13;
		};

		template<class R, class C, class P0, class P1, class P2, class P3, class P4, class P5, class P6, class P7, class P8, class P9, class P10, class P11, class P12>
		struct function_types< R( C::* )(P0, P1, P2, P3, P4, P5, P6, P7, P8, P9, P10, P11, P12) const>
		{
			typedef R ret_type;
			typedef C class_type;

			typedef typename external_type_cast<P0>::Type param0;
			typedef typename external_type_cast<P1>::Type param1;
			typedef typename external_type_cast<P2>::Type param2;
			typedef typename external_type_cast<P3>::Type param3;
			typedef typename external_type_cast<P4>::Type param4;
			typedef typename external_type_cast<P5>::Type param5;
			typedef typename external_type_cast<P6>::Type param6;
			typedef typename external_type_cast<P7>::Type param7;
			typedef typename external_type_cast<P8>::Type param8;
			typedef typename external_type_cast<P9>::Type param9;
			typedef typename external_type_cast<P10>::Type param10;
			typedef typename external_type_cast<P11>::Type param11;
			typedef typename external_type_cast<P12>::Type param12;

			static const bool method = true;
			static const size_t arity = 13;
		};
	}

	template<class F>
	struct function_traits
	{
		typedef detail::function_types<F> result;
	};
}

