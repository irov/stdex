#   pragma once

namespace stdex
{
    class intrusive_ptr_base
    {
    public:
        intrusive_ptr_base()
            : m_reference(0)
        {
        }

    protected:
        virtual void destroy()
        {
            //Empty
        }

    protected:
        friend void intrusive_ptr_add_ref( intrusive_ptr_base * _ptr );
        friend void intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr );

    protected:
        size_t m_reference;
    };
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_add_ref( intrusive_ptr_base * _ptr )
    {
        ++_ptr->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_dec_ref( intrusive_ptr_base * _ptr )
    {
        if( --_ptr->m_reference == 0 )
        {
            _ptr->destroy();
        }
    }
}