#   pragma once

#   include <stddef.h>

#	include "stdex/intrusive_ptr.h"

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

#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
		inline static bool intrusive_ptr_check_ref( intrusive_ptr_base<T> * _ptr );
#	endif

    protected:
        size_t m_reference;
    };
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline void intrusive_ptr_setup( T *& _ptr, T * _other )
	{
		_ptr = _other;

		if( _other != nullptr )
		{
			T::intrusive_ptr_add_ref( _other );
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
			T::intrusive_ptr_dec_ref( _ptr );
			_ptr = nullptr;
		}
	}
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
	//////////////////////////////////////////////////////////////////////////
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
	//////////////////////////////////////////////////////////////////////////
	template<class T>
	inline bool intrusive_ptr_base<T>::intrusive_ptr_check_ref( intrusive_ptr_base<T> * _ptr )
	{
		if( _ptr->m_reference == 0 )
		{
			return false;
		}

		return true; 
	}
#	endif
}