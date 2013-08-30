#	pragma once

#	include "stdex/intrusive_ptr.h"
#	include "stdex/intrusive_linked.h"

#   include <stddef.h>

namespace stdex
{
    class const_string_holder
        : public intrusive_linked<const_string_holder>
    {
    protected:
        const_string_holder()
            : m_reference(0)
            , m_data(nullptr)
            , m_hash(0)
            , m_size(0)
        {
            m_owner = this;
        }

    private:
        const_string_holder & operator = ( const const_string_holder & _holder )
        {
            (void)_holder;

            return *this;
        }

    public:
        inline const char * c_str() const
        {
            return m_owner->_c_str();
        }

        inline size_t size() const
        {
            return m_size;
        }

        inline bool empty() const
        {
            return m_size == 0;
        }

    protected:
        const char * _c_str() const
        {
            return m_data;
        }

    protected:
        void releaseString()
        {
            m_data = nullptr;

            this->_releaseString();
        }

        virtual void _releaseString() = 0;

    protected:
        void destroyString()
        {
            m_data = nullptr;

            this->_destroyString();
        }

        virtual void _destroyString() = 0;

    public:
        inline const_string_holder * owner() const
        {
            return m_owner;
        }

        inline size_t hash() const
        {
            return m_hash;
        }

    public:
        bool less( const_string_holder * _holder );
        bool equal( const_string_holder * _holder );

    public:
        void combine( const_string_holder * _holder );

    protected:        
        class ForeachCombineOwner;
        class ForeachCombineOther;

        void combine_owner( const_string_holder * _out );
        void combine_other( const_string_holder * _out );
        void combine_from( const_string_holder * _from, const_string_holder * _out );

    protected:
        friend inline void intrusive_ptr_add_ref( const_string_holder * _ptr );
        friend inline void intrusive_ptr_dec_ref( const_string_holder * _ptr );

    protected:
        void setup( const char * _data, size_t _size );

    protected:
        size_t m_reference;

        const char * m_data;

        size_t m_hash;
        size_t m_size;

        mutable const_string_holder * m_owner;
    };
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_add_ref( const_string_holder * _ptr )
    {
        ++_ptr->m_reference;
        ++_ptr->m_owner->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    inline void intrusive_ptr_dec_ref( const_string_holder * _ptr )
    {
        if( --_ptr->m_owner->m_reference == 0 )
        {
            _ptr->m_owner->destroyString();
        }

        if( --_ptr->m_reference == 0 )
        {
            _ptr->destroyString();
        }
    }
}