#pragma once

#include "stdex/intrusive_ptr_base.h"

namespace stdex
{
    class intrusive_ptr_scope
    {
    public:
        explicit intrusive_ptr_scope( intrusive_ptr_base * _ptr )
            : m_ptr( _ptr )
        {
            intrusive_ptr_base::intrusive_ptr_add_ref( m_ptr );
        }

        ~intrusive_ptr_scope()
        {
            intrusive_ptr_base::intrusive_ptr_dec_ref( m_ptr );
        }

    protected:
        intrusive_ptr_base * m_ptr;
    };
}