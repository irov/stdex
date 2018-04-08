#	pragma once

#	include "stdex/intrusive_ptr.h"
#	include "stdex/intrusive_ptr_base.h"

#   include <stddef.h>
#	include <stdint.h>

namespace stdex
{
    class const_string2_holder
        : public intrusive_ptr_base<const_string2_holder>
    {
    protected:
        const_string2_holder();
        const_string2_holder( const const_string2_holder & _holder );
        
	public:
		typedef size_t size_type;
		typedef int64_t hash_type;
        typedef char value_type;

    public:
        inline const char * c_str() const
        {
            return m_data;
        }

        inline size_type size() const
        {
            return m_size;
        }

        inline bool empty() const
        {
            return m_size == 0;
        }

    protected:
        virtual void destroyString() = 0;

    public:
        inline hash_type hash() const
        {
            return m_hash;
        }

    protected:
		friend class stdex::intrusive_ptr<const_string2_holder>;

    public:
        inline static void intrusive_ptr_destroy( const_string2_holder * _ptr );

    public:
		void setup( const char * _data, size_type _size, hash_type _hash );

    protected:
        const char * m_data;
		size_type m_size;

        hash_type m_hash;
    };
    //////////////////////////////////////////////////////////////////////////
    inline void const_string2_holder::intrusive_ptr_destroy( const_string2_holder * _ptr )
    {
        _ptr->destroyString();
        //Empty
    }
}