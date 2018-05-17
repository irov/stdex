#pragma once

#include <stdint.h>

#include "stdex/intrusive_ptr.h"

namespace stdex
{
    class intrusive_ptr_base
    {
    public:
        intrusive_ptr_base()
            : m_reference( 0 )
        {
        }

        virtual ~intrusive_ptr_base()
        {
        }

    public:
        inline static void intrusive_ptr_add_ref( intrusive_ptr_base * _ptr );
        inline static void intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr );
        inline static uint32_t intrusive_ptr_get_ref( const intrusive_ptr_base * _ptr );
        
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
    public:
        inline static bool intrusive_ptr_check_ref( intrusive_ptr_base * _ptr );
#	endif

    protected:
        virtual void destroy() = 0;

    protected:
        uint32_t m_reference;
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void intrusive_ptr_setup( T *& _ptr, T * _other )
    {
        _ptr = _other;

        if( _other != nullptr )
        {
            intrusive_ptr_base::intrusive_ptr_add_ref( _other );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void intrusive_ptr_setup( T *& _ptr, const stdex::intrusive_ptr<T> & _other )
    {
        T * other_ptr = _other.get();

        stdex::intrusive_ptr_setup( _ptr, other_ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    inline void intrusive_ptr_release( T *& _ptr )
    {
        if( _ptr != nullptr )
        {
            intrusive_ptr_base::intrusive_ptr_dec_ref( _ptr );
            _ptr = nullptr;
        }
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_this_acquire( intrusive_ptr_base * _ptr )
    {
        intrusive_ptr_base::intrusive_ptr_add_ref( _ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_this_release( intrusive_ptr_base * _ptr )
    {
        intrusive_ptr_base::intrusive_ptr_dec_ref( _ptr );
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_base::intrusive_ptr_add_ref( intrusive_ptr_base * _ptr )
    {
        ++_ptr->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_base::intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr )
    {
        if( --_ptr->m_reference == 0 )
        {
            _ptr->destroy();
        }
    }
    //////////////////////////////////////////////////////////////////////////
    inline uint32_t intrusive_ptr_base::intrusive_ptr_get_ref( const intrusive_ptr_base * _ptr )
    {
        return _ptr->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
    //////////////////////////////////////////////////////////////////////////
    inline bool intrusive_ptr_base::intrusive_ptr_check_ref( intrusive_ptr_base * _ptr )
    {
        if( _ptr->m_reference == 0 )
        {
            return false;
        }

        return true;
    }
#	endif
}