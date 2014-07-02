#   pragma once

#   include <stddef.h>

namespace stdex
{
	template<class T>
    class intrusive_ptr_base
    {
    public:
        intrusive_ptr_base()
            : m_reference(0)
        {
        }

	public:
        inline static void intrusive_ptr_add_ref( intrusive_ptr_base<T> * _ptr );
        inline static void intrusive_ptr_dec_ref( intrusive_ptr_base<T> * _ptr );

    protected:
        size_t m_reference;
    };
    //////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void intrusive_ptr_base<T>::intrusive_ptr_add_ref( intrusive_ptr_base<T> * _ptr )
    {
        ++_ptr->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
	template<class T>
    inline void intrusive_ptr_base<T>::intrusive_ptr_dec_ref( intrusive_ptr_base<T> * _ptr )
    {
        if( --_ptr->m_reference == 0 )
        {
			T * t = static_cast<T *>(_ptr);

			t->intrusive_ptr_destroy();
        }
    }
}