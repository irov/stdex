#pragma once

#include "stdex/intrusive_ptr_config.h"

namespace stdex
{
    class intrusive_ptr_base
    {
    public:
        virtual unsigned int incref() noexcept = 0;
        virtual void decref() noexcept = 0;
        virtual unsigned int getrefcount() const noexcept = 0;

    public:
        inline static void intrusive_ptr_add_ref( intrusive_ptr_base * _ptr ) noexcept;
        inline static void intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr ) noexcept;
        inline static unsigned int intrusive_ptr_get_ref( const intrusive_ptr_base * _ptr ) noexcept;

        template<class T>
        static void intrusive_ptr_setup( T *& _ptr, T * _other );

        template<class T>
        static void intrusive_ptr_release( T *& _ptr );

#ifdef STDEX_INTRUSIVE_PTR_DEBUG
    public:
        inline static bool intrusive_ptr_check_ref( const intrusive_ptr_base * _ptr ) noexcept;
#endif
    };
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void intrusive_ptr_base::intrusive_ptr_setup( T *& _ptr, T * _other )
    {
        _ptr = _other;

        if( _other != nullptr )
        {
            intrusive_ptr_base::intrusive_ptr_add_ref( _other );
        }
    }
    //////////////////////////////////////////////////////////////////////////
    template<class T>
    void intrusive_ptr_base::intrusive_ptr_release( T *& _ptr )
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
    inline void intrusive_ptr_base::intrusive_ptr_add_ref( intrusive_ptr_base * _ptr ) noexcept
    {
        _ptr->incref();
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_base::intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr ) noexcept
    {
        _ptr->decref();
    }
    //////////////////////////////////////////////////////////////////////////
    inline unsigned int intrusive_ptr_base::intrusive_ptr_get_ref( const intrusive_ptr_base * _ptr ) noexcept
    {
        unsigned int refcount = _ptr->getrefcount();

        return refcount;
    }
    //////////////////////////////////////////////////////////////////////////
#ifdef STDEX_INTRUSIVE_PTR_DEBUG
    //////////////////////////////////////////////////////////////////////////
    inline bool intrusive_ptr_base::intrusive_ptr_check_ref( const intrusive_ptr_base * _ptr ) noexcept
    {
        if( _ptr == nullptr )
        {
            return false;
        }

        unsigned int refcount = _ptr->getrefcount();

        if( refcount == 0 )
        {
            return false;
        }

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
#endif
}