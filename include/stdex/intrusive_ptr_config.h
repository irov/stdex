#pragma once

#include "stdex/exception.h"

#include <cstddef>
#include <cstdint>

#ifndef STDEX_INTRUSIVE_PTR_DEBUG_ENABLE
#   ifndef NDEBUG
#       define STDEX_INTRUSIVE_PTR_DEBUG
#   endif
#else
#   define STDEX_INTRUSIVE_PTR_DEBUG
#endif

#if defined(WIN32) && !defined(NDEBUG)

#pragma warning(disable:6011)

inline void stdex_intrusive_ptr_critical_crash_error()
{
    volatile unsigned int * p = nullptr;
    *p = 0xBADF00D;
}

#pragma warning(default:6011)

#define STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR stdex_intrusive_ptr_critical_crash_error()
#else
#define STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR 
#endif

#ifdef STDEX_INTRUSIVE_PTR_DEBUG
template<class V, class T>
void stdex_intrusive_ptr_check_typecast_ptr( T _ptr )
{
    if( _ptr != nullptr && dynamic_cast<V>(_ptr) == nullptr )
    {
        STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR;
        STDEX_THROW_EXCEPTION( "ptr invalid cast" );
    }
}

template<class T>
void stdex_intrusive_ptr_check_debug_mask( T _ptr )
{
    if( _ptr->m_debug_ptr_mask__ != 0xABCDEF01 )
    {
        STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR;
        STDEX_THROW_EXCEPTION( "mask != 0xABCDEF01" );
    }
}

#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE) stdex_intrusive_ptr_check_typecast_ptr<TYPE>( PTR )
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK() public: uint32_t m_debug_ptr_mask__; protected:
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK() this->m_debug_ptr_mask__ = 0xABCDEF01
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK() stdex_intrusive_ptr_check_debug_mask( this )
#else
#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE)
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK()
#endif
