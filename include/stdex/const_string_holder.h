#	pragma once

#	include "stdex/intrusive_ptr.h"
#	include "stdex/intrusive_linked.h"
#	include "stdex/thread_guard.h"

#   include <stddef.h>
#	include <stdint.h>

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
			, m_combine(true)
        {
            m_owner = this;
        }

    private:
        void operator = ( const const_string_holder & )
        {
        }

	public:
		typedef size_t size_type;
		typedef long hash_type;

    public:
        inline const char * c_str() const
        {
			const char * data = m_owner->m_data;

            return data;
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
        void releaseString( const char * _data )
        {
            m_data = _data;

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

        inline hash_type hash() const
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

        void combine_owner_( const_string_holder * _out );
        void combine_other_( const_string_holder * _out );
        void combine_from_( const_string_holder * _from, const_string_holder * _out );
		
    protected:
		friend class stdex::intrusive_ptr<const_string_holder>;

        inline static void intrusive_ptr_add_ref( const_string_holder * _ptr );
        inline static void intrusive_ptr_dec_ref( const_string_holder * _ptr );

#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
		inline static bool intrusive_ptr_check_ref( const_string_holder * _ptr );
#	endif

    protected:
		void setup( const char * _data, size_type _size, hash_type _hash, bool _combine );

    protected:
        uint32_t m_reference;

        const char * m_data;
		size_type m_size;

        hash_type m_hash;

        mutable const_string_holder * m_owner;

		bool m_combine;

		STDEX_THREAD_GUARD_INIT;
    };
    //////////////////////////////////////////////////////////////////////////
	inline void const_string_holder::intrusive_ptr_add_ref( const_string_holder * _ptr )
    {
		STDEX_THREAD_GUARD_CHECK( _ptr, "intrusive_ptr_add_ref" );
        ++_ptr->m_reference;
        ++_ptr->m_owner->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    inline void const_string_holder::intrusive_ptr_dec_ref( const_string_holder * _ptr )
    {
		STDEX_THREAD_GUARD_CHECK( _ptr, "intrusive_ptr_dec_ref" );

        if( --_ptr->m_owner->m_reference == 0 )
        {
            _ptr->m_owner->destroyString();
        }

        if( --_ptr->m_reference == 0 )
        {
            _ptr->destroyString();
        }
    }
#	ifdef STDEX_INTRUSIVE_PTR_DEBUG
	//////////////////////////////////////////////////////////////////////////
	inline bool const_string_holder::intrusive_ptr_check_ref( const_string_holder * _ptr )
	{
		if( _ptr->m_owner->m_reference == 0 )
		{
			return false;
		}

		if( _ptr->m_reference == 0 )
		{
			return false;
		}

		return true; 
	}
#	endif
}