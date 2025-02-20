#pragma once

#if !defined(STDEX_INTRUSIVE_PTR_DEBUG_ENABLE)
#   ifndef NDEBUG
#       define STDEX_INTRUSIVE_PTR_DEBUG
#   endif
#else
#   define STDEX_INTRUSIVE_PTR_DEBUG
#endif

#if defined(STDEX_INTRUSIVE_PTR_DEBUG)
#   include <cassert>

#   define STDEX_INTRUSIVE_PTR_DEBUG_MASK (0x7BADF00D)

namespace stdex
{
    inline void __intrusive_ptr_critical_crash_error()
    {
        assert("intrusive_ptr_critical_crash_error");
    }

    template<class V, class T>
    void __intrusive_ptr_check_typecast_ptr( T _ptr )
    {
        if( _ptr != nullptr && dynamic_cast<V>(_ptr) == nullptr )
        {
            stdex::__intrusive_ptr_critical_crash_error();
        }
    }

    template<class T>
    void __intrusive_ptr_check_debug_mask( T _ptr )
    {
        if( _ptr->m_debug_ptr_mask__ != STDEX_INTRUSIVE_PTR_DEBUG_MASK )
        {
            stdex::__intrusive_ptr_critical_crash_error();
        }
    }
}

#   define STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR stdex::__intrusive_ptr_critical_crash_error()
#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE) stdex::__intrusive_ptr_check_typecast_ptr<TYPE>( PTR )
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK() public: unsigned int m_debug_ptr_mask__; protected:
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK() this->m_debug_ptr_mask__ = STDEX_INTRUSIVE_PTR_DEBUG_MASK
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK() stdex::__intrusive_ptr_check_debug_mask( this )
#else
#   define STDEX_INTRUSIVE_PTR_CRITICAL_CRASH_ERROR
#   define STDEX_INTRUSIVE_PTR_CHECK_TYPECAST_PTR(PTR, TYPE)
#   define STDEX_INTRUSIVE_PTR_DECLARE_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_INIT_DEBUG_MASK()
#   define STDEX_INTRUSIVE_PTR_CHECK_DEBUG_MASK()
#endif
