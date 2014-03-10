#	include "stdex/const_string_holder.h"

#   include <string.h>

namespace stdex
{    
    //////////////////////////////////////////////////////////////////////////
    bool const_string_holder::less( const_string_holder * _holder )
    {
        if( m_hash < _holder->m_hash )
        {
            return true;
        }

        if( m_hash > _holder->m_hash )
        {
            return false;
        }

        if( m_owner == _holder->m_owner )
        {
            return false;
        }

        const char * left = this->c_str();
        const char * right = _holder->c_str();

        int res = strcmp( left, right );

        if( res == 0 )
        {
            this->combine( _holder );

            return false;
        }

        return res < 0;
    }
    //////////////////////////////////////////////////////////////////////////
    bool const_string_holder::equal( const_string_holder * _holder )
    {
        if( m_owner == _holder->m_owner )
        {
            return true;
        }

        if( m_hash != _holder->m_hash )
        {
            return false;
        }

        const char * left = this->c_str();
        const char * right = _holder->c_str();

        if( strcmp( left, right ) != 0 )
        {
            return false;
        }

        this->combine( _holder );

        return true;
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder::setup( const char * _data, size_t _size, bool _combine )
    {
        m_data = _data;
        m_hash = const_string_make_hash( m_data );
        m_size = _size;
		m_combine = _combine;
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder::combine( const_string_holder * _holder )
    {
		if( m_combine == false || _holder->m_combine == false )
		{
			return;
		}

        if( m_owner->m_reference > _holder->m_owner->m_reference )
        {
            this->combine_from_( _holder, this );
        }
        else
        {
            this->combine_from_( this, _holder );
        }
    }
    //////////////////////////////////////////////////////////////////////////       
    class const_string_holder::ForeachCombineOwner
    {
    public:
        ForeachCombineOwner( const_string_holder * _out )
            : m_out(_out)
        {
        }

    public:
        void operator () ( intrusive_linked<const_string_holder> * _linked ) const
        {
            const_string_holder * elem = static_cast<const_string_holder *>(_linked);

            elem->combine_owner_( m_out );
        }

    protected:
        const_string_holder * m_out;
    };
    //////////////////////////////////////////////////////////////////////////
    class const_string_holder::ForeachCombineOther
    {
    public:
        ForeachCombineOther( const_string_holder * _out )
            : m_out(_out)
        {
        }

    public:
        void operator () ( intrusive_linked<const_string_holder> * _linked ) const
        {
            const_string_holder * elem = static_cast<const_string_holder *>(_linked);

            elem->combine_other_( m_out );
        }

    protected:
        const_string_holder * m_out;
    };
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder::combine_owner_( const_string_holder * _out )
    {
        this->m_owner = _out->m_owner;
        this->m_reference >>= 1;

        _out->m_owner->m_reference += this->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder::combine_other_( const_string_holder * _out )
    {
        this->m_owner->m_reference -= this->m_reference;
        this->m_owner = _out->m_owner;

        _out->m_owner->m_reference += this->m_reference;
    }
    //////////////////////////////////////////////////////////////////////////
    void const_string_holder::combine_from_( const_string_holder * _from, const_string_holder * _out )
    {
		const_string_holder * from_owner = _from->m_owner;

		const char * out_data = _out->m_owner->c_str();
        from_owner->releaseString( out_data );

        if( _from->unique() == true )
        {
            ForeachCombineOwner combineowner(_out);
            _from->foreach_self( combineowner );
        }
        else
        {
            ForeachCombineOther combineother(_out);
            from_owner->foreach_other( combineother );

            if( from_owner->m_reference == 0 )
            {
                from_owner->destroyString();
            }
            else
            {
                ForeachCombineOwner combineowner(_out);
                from_owner->foreach_self( combineowner );						
            }
        }

        _from->linkall( _out );
    }
}